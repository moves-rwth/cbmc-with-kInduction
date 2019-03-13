import hashlib
import os
import time

from lxml import etree


def get_data_witness_type(text: str):
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='witness-type')
	elm.text = text
	elm.text = text
	return elm


def get_data_specification(text: str):
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='specification')
	elm.text = text
	return elm


def get_data_programhash(text: str):
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='programhash')
	elm.text = text
	return elm


def get_data_architecture(text: str):
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='architecture')
	elm.text = text
	return elm


def get_data_producer(text: str):
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='producer')
	elm.text = text
	return elm


def get_data_programfile(input_file: str):
	text = os.path.basename(input_file)
	elm = etree.Element('{http://graphml.graphdrawing.org/xmlns}data', key='programfile')
	elm.text = text
	return elm


def get_sha512_hash(input_file: str) -> str:
	h = hashlib.sha256()
	with open(input_file, 'rb') as f:
		h.update(f.read())
	return h.hexdigest()


def write_back(graph, file):
	s = etree.tostring(graph, pretty_print=True)
	with open(file, 'wb') as f:
		f.write(s)


def extend_from_cbmc_to_cpa_format(filename: str, input_file: str, result: bool):
	time.sleep(10)
	parser = etree.XMLParser(remove_blank_text=True)
	tree = etree.parse(filename, parser)
	g = tree.find('{http://graphml.graphdrawing.org/xmlns}graph')

	g.append(get_data_witness_type('correctness_witness' if result else 'violation_witness'))
	g.append(get_data_specification('CHECK( init(main()), LTL(G ! call(__VERIFIER_error())) )'))
	g.append(get_data_programhash(get_sha512_hash(input_file)))
	g.append(get_data_architecture('32bit'))
	g.append(get_data_producer('CBMC'))
	g.append(get_data_programfile(input_file))

	write_back(tree, filename)


# extend_from_cbmc_to_cpa_format('ecc-REQ-235600-False.c_base.gml', 'canalyzer.py')

