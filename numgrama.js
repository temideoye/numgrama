/*
 * Numgrama.
 *
 * Returns the gramatically correct English phrase of input decimal number.
 * Decimal range: [1, 1001)
 *
 * For range > 1000, include the following number notations in groups:
 *   googolplex googol octillion septillion sextillion
 *   quintillion quadrillion trillion billion million
 *
 * Copyright: June 2019, Temi Adeoye
 * License: See LICENSE for details
 */
const units = 'zero one two three four five six seven eight nine'.split(' ');
const earlytens = 'ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen'.split(
    ' '
);
const latetens = 'twenty thirty fourty fifty sixty seventy eighty ninety'.split(' ');
const groups = 'thousand'.split(' '); // for range [1, 1001)

const seperator = digits => {
    // Seperate thousands into list container. e.g ['1', '000'] for 1000.
    let strdigits = `${digits}`;
    const sep = [];
    while (strdigits.length > 3) {
        sep.unshift(strdigits.slice(-3, strdigits.length));
        strdigits = strdigits.slice(0, -3);
    }
    if (strdigits) sep.unshift(strdigits); // if strdigits not empty at the end of loop

    return sep;
};

const cardinality = num => {
    // Calculate cardinality of input digits.
    const result = [];
    const ones = num % 10;
    const tens = num % 100;
    const hundreds = num % 1000;
    const inEarlytens = tens > 9 && tens < 20; // between 10 and 19
    const inLatetens = tens > 19 && tens < 100; // between 20 and 99
    const joinable = inEarlytens || inLatetens || ones; // between 1 and 99
    const roundHundred = hundreds - tens; // round down to nearest hundred

    if (roundHundred) result.push(`${units[Math.floor(roundHundred / 100)]} hundred`);
    if (roundHundred && joinable) result.push('and');
    if (inEarlytens) result.push(earlytens[tens - 10]);
    if (inLatetens) {
        const netTens = latetens[Math.floor(tens / 10) - 2]; // latetens indexed 2 places left
        if (ones) result.push(`${netTens}-${units[ones]}`);
        else result.push(netTens);
    }
    if (ones && !(inEarlytens || inLatetens)) result.push(units[ones]);

    return result.join(' ');
};

const toTitleCase = str => {
    // Convert string to title case.
    const result = [];
    str.split(' ').forEach(word => result.push(`${word[0].toUpperCase()}${word.slice(1)}`));
    return result.join(' ');
};

const main = arg => {
    // Compute english phrase.
    const strarg = `${arg}`.replace(',', ''); // strip comma from argument
    // for range [1, 1001)
    if (parseFloat(strarg) > 1000) {
        console.log('Acceptable input decimal range is [1, 1001)');
        return;
    }

    let whole = strarg;
    let fractional = false;

    if (strarg.includes('.')) [whole, fractional] = strarg.split('.');

    const sep = seperator(whole);
    const seplength = sep.length;
    const result = [];

    sep.forEach((value, group) => {
        const intvalue = parseInt(value, 10);
        result.push(cardinality(intvalue));
        // append group, ignoring hundreds and 'zero thousands'
        if (intvalue && seplength > 1 && group !== seplength - 1) {
            result.push(groups.slice(1 - seplength)[group]);
        }
    });

    if (fractional) {
        result.push('point');
        fractional.split('').forEach(digit => {
            const intdigit = parseInt(digit, 10);
            if (intdigit === 0) result.push('zero');
            else result.push(cardinality(intdigit));
        });
    }

    console.log(toTitleCase(result.join(' ').trim()));
};

if (require.main === module) {
    try {
        main(process.argv[2]);
    } catch (err) {
        if (process.argv.length < 3) {
            console.log('Numgrama takes exactly one argument, none was provided.');
        }
    }
}
