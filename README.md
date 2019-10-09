# k-Induction (for CBMC)

The goal of this python tool is to enable any C bounded model checker, such as [CBMC] (https://www.cprover.org/cbmc), to be employed for k-Induction over one-loop embedded-style C programs (see below for a description).  
For now, the tool was only tested and used to extend CBMC with k-induction proofs.

## Prerequisites

You will need `pycparser`, `pycparserext`, `pyyaml` and `psutil`. All packages are conveniently available via `pip`, 
although it is recommended that a current version of `pycparserext` is installed from [source]
(https://github.com/inducer/pycparserext), as the PyPI version is quite outdated.

For dealing with generated witnesses, the package `lxml` is necessary as well.  

You will also need a bounded model checker to your liking.

## Usage

### Input programs

The k-Induction tool **only** runs on embedded-style C programs and **does not** take arbitrary C files as input. A 
typical example of a k-Induction verification task  looks like this:

```c
extern void __VERIFIER_error();

void initialize() {
	// Variable initialization
}

void step() {
	// Bounded step
}

void main() {
	initialize();
	while (1) {
		step();
		if (property_violated()) {
			__VERIFIER_error();
		}
	}
}
```

The following requirements are imposed on any C input file:

* It shall have exactly one loop directly inside the main function.
* The loop shall contain exactly one property violation check as its very last statement.
* The property check shall be either done by an `assert()` statement or a `__VERIFIER_error()` call guarded by multiple 
if statements.
* Any execution of`initialize()` and `step()` shall be bounded.

### Supported verifiers

In theory, the k-Induction tool can use any bounded model checker or incremental bounded model checker for C as its backend, as long as the usage of the verifier is correctly configured (see section below for details).  
Note that during the development process, only CBMC was used to test the tool's functionality.

One needs to specify:

* Whether the verifier works incrementally or in a standard BMC configuration.
* The call string for the base step and the call string for the induction step. This tells the tool how to run the 
verifier. The base call is just a standard BMC configuration, whereas the induction call shall have the inverse 
termination behavior, i.e. terminates iff no counterexample could be found. If the verifier is not working 
incrementally, use `KINCREMENT` at the position where the unwind depth should be inserted.
* Regular expressions to detect true and false instances in the output of the verifier. They shall follow the python 
`re` module [specifications](https://docs.python.org/3/library/re.html).
* If the verifier is working incrementally, the k-Induction tool needs to identify the current number of unwinds from 
the verifier output. This can also be set using a regex, where the number of regex matches is taken as the unwind 
number.

### Configuration

We deliver a standard configuration for CBMC in a BMC settings as well as CBMC in an incremental setting. You can find 
both configuration files in the `config` folder. An exemplary configuration for CBMC with explanatory comments:

```yaml
# Contains all informations about how to call the verifier and interpret its output.
verifier:
   # Whether the verifier is an incremental BMC.
  incremental       : no
  # How to call the model checker on the base file. This is a standard BMC configuration.
  base_call         : "cbmc --unwindset main.0:KINCREMENT --no-unwinding-assertions"
  # How to call the model checker on the induction file.
  # This shall have the inverted termination behavior of the standard configuration.
  induction_call    : "cbmc --unwindset main.0:KINCREMENT --no-unwinding-assertions"
  # The string that indicates where in the calls to insert the current iteration number.
  # It will be replaced with an integer.
  k_increment_string: "KINCREMENT"
  # Contains all information on how to interpret the verifier's output.
  output:
    # How to detect a "false" result.
    false_regex         : "VERIFICATION FAILED"
    # How to detect a "true" result.
    true_regex          : "VERIFICATION SUCCESSFUL"
    # How to count the number of iterations done, which is the number of occurrences of
    # the regular expression.
    k_regex             : "VERIFICATION FAILED|VERIFICATION SUCCESSFUL"
    # If SMT time is measured, this is a regex that comes before the SMT time that
    # is printed by the verifier.
    smt_time_start_regex: "Runtime decision procedure: "
    # Same as above, but a regex that comes after the SMT time.
    smt_time_end_regex  : "s"
    # How ofter (in seconds) to poll for verifier output. Can be float value.
    poll_interval       : 2

# Contains all information on the specifics of the input C file.
input:
  # The main entry function.
  main_function  : "main"
  # The assert function.
  assert_function: "assert"
  # The verifier assume function.
  assume_function: "__VERIFIER_assume"
  # The verifier error function.
  error_function : "__VERIFIER_error"
```

Pass the configuration file location via the `-c` or `--config` option. Configurations for other C bounded model 
checkers, such as ESBMC or SMACK, can be added easily.

### Verification

To verify `file.c`, run `python kinduction.py -c ../config/cbmc.yaml file.c` from the `src` folder.

#### Options

* `-t TIMELIMIT, --timelimit TIMELIMIT` The maximum CPU-time [s] for the verification.
* `--smt-time` Prints out the time that was spent on SMT-solving. Works only if the bounded model checker is configured 
accordingly.

## How it works

k-Induction can be simulated using bounded model checking by means of parallely checking the base case and the induction
case. If the base case finds a counterexample, we stop and return `False`. If the induction case finds a proof on 
iteration `k` and the base case found no counterexample up to iteration `k`, we stop and return `True`.

The base case is just a standard BMC configuration, where it is checked whether a counterexample can be found up to 
depth  `k`.

The induction case tries to find an inductive proof by setting all variables modified in the loop to a non-deterministic
value before the loop execution. This simulates entering the loop at an arbitrary point in the program execution. It is 
then to show that, under the assumption that on the beginning of the loop the property was valid, no violation of the 
property can be reached at the end of the loop.

For a standard BMC configuration and the exemplary input program from above, the code transformation of the k-Induction 
tool in the `k`-th iteration looks roughly as follows:

```c
extern void __VERIFIER_error();

void initialize() {
	// Variable initialization
}

void step() {
	// Bounded step
}

void main() {
	unsigned int i = 0;
	initialize();
	set_variables_modified_in_loop_to_nondet_value();
	while (1) {
		__VERIFIER_assume(! property_violated());
		i++;
		step();
		if (i == k && property_violated()) {
			__VERIFIER_error();
		}
	}
}
```
