import unittest


class WitnessTest(unittest.TestCase):
    def test(self):
        self.assertEqual('foo'.upper(), 'FOO')


if __name__ == '__main__':
    unittest.main()
