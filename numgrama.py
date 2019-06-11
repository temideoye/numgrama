"""
Numgrama.

Returns the gramatically correct English phrase of input decimal number.
Default decimal range: [1, 1001)

For range > 1000, include the following number notations in groups:
    googolplex googol octillion septillion sextillion
    quintillion quadrillion trillion billion million

Copyright: June 2019, Temi Adeoye
License: See LICENSE for details
"""

units = 'zero one two three four five six seven eight nine'.split()
earlytens = 'ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen'.split()
latetens = 'twenty thirty fourty fifty sixty seventy eighty ninety'.split()
groups = 'thousand'.split()  # for range [1, 1001)


def seperator(digits):
    """Seperate thousands into list container. e.g ['1', '000'] for 1000."""
    strdigits = str(digits)
    sep = []
    while len(strdigits) > 3:
        sep.insert(0, strdigits[-3: len(strdigits)])
        strdigits = strdigits[0:-3]
    # if strdigits not empty at the end of loop
    if strdigits: sep.insert(0, strdigits)

    return sep


def cardinality(num):
    """Calculate cardinality of input digits."""
    result = []
    ones, tens, hundreds = num % 10, num % 100, num % 1000
    in_earlytens = 9 < tens < 20  # between 10 and 19
    in_latetens = 19 < tens < 100  # between 20 and 99
    joinable = in_earlytens or in_latetens or ones  # between 1 and 99
    round_hundred = hundreds - tens  # round down to nearest hundred

    if round_hundred: result.append(units[round_hundred // 100] + ' hundred')
    if round_hundred and joinable: result.append('and')
    if in_earlytens: result.append(earlytens[tens - 10])
    if in_latetens:
        net_tens = latetens[(tens // 10) - 2]  # latetens indexed 2 places left
        if ones: result.append(net_tens + '-' + units[ones])
        else: result.append(net_tens)
    if ones and not (in_earlytens or in_latetens): result.append(units[ones])

    return ' '.join(result)


def main(arg):
    """Compute english phrase."""
    strarg = str(arg).replace(',', '')  # strip comma from argument
    if float(strarg) > 1000:  # for range [1, 1001)
        print('Acceptable input decimal range is [1, 1001)')
        return

    whole = strarg
    fractional = False

    if '.' in strarg: whole, fractional = strarg.split('.')

    sep = seperator(whole)
    seplength = len(sep)
    result = []

    for group, value in enumerate(sep):
        result.append(cardinality(int(value)))
        # append group, ignoring hundreds and 'zero thousands'
        if int(value) and seplength > 1 and group != seplength - 1:
            result.append(groups[1 - seplength:][group])

    if fractional:
        result.append('point')
        for digit in fractional:
            if int(digit) == 0: result.append('zero')
            else: result.append(cardinality(int(digit)))

    print(' '.join(result).title().strip())


if __name__ == '__main__':
    from sys import argv
    try: main(argv[1])
    except IndexError: print('Numgrama takes one argument, none was provided.')
