# k-Induction

This python tool enables any C bounded model checker, such as [CBMC] (https://www.cprover.org/cbmc), to be employed for 
k-Induction over one-loop embedded-style C programs.

## Prerequisites

You will need `pycparser`, `pycparserext` and `psutil`. All packages are conveniently available via `pip`, although it
is recommended that a current version of `pycparserext` is installed from [source]
(https://github.com/inducer/pycparserext), as the PyPI version is quite outdated.

## Usage

### Input programs

The k-Induction tool **only** runs on embedded-style C programs and **does not** take arbitrary C files as input. A 
typical example of a k-Induction verification task  looks like this:

```C
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
* The loop shall contain exactly one property violation check.
* The property check shall be either done by an `assert()` statement or a `__VERIFIER_error()` call guarded by exactly one
if statement.
* Any execution of`initialize()` and `step()` shall be bounded.

### Supported verifiers

The k-Induction tool can use any bounded model checker or incremental bounded model checker for C as its backend, as 
long as the usage of the verifier is correctly configured. For this, one needs to specify:

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

We deliver a standard configuration for CBMC in a BMC settings as well as CBMC in an incremental setting.

### Verification

To verify `file.c`, run `python kinduction.py file.c`. A timeout, in seconds, can be set by using `-t`.

## How it works