// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*=============================================================================
**
** Source: test1.c
**
** Purpose: Test to ensure that fabsf return the correct values
** 
** Dependencies: PAL_Initialize
**               PAL_Terminate
**               Fail
**
**===========================================================================*/

#include <palsuite.h>

// binary32 (float) has a machine epsilon of 2^-23 (approx. 1.19e-07). However, this 
// is slightly too accurate when writing tests meant to run against libm implementations
// for various platforms. 2^-21 (approx. 4.76e-07) seems to be as accurate as we can get.
//
// The tests themselves will take PAL_EPSILON and adjust it according to the expected result
// so that the delta used for comparison will compare the most significant digits and ignore
// any digits that are outside the double precision range (6-9 digits).

// For example, a test with an expect result in the format of 0.xxxxxxxxx will use PAL_EPSILON
// for the variance, while an expected result in the format of 0.0xxxxxxxxx will use
// PAL_EPSILON / 10 and and expected result in the format of x.xxxxxx will use PAL_EPSILON * 10.
#define PAL_EPSILON 4.76837158e-07
                    

#define PAL_NAN     sqrt(-1.0)
#define PAL_POSINF -log(0.0)
#define PAL_NEGINF  log(0.0)

/**
 * Helper test structure
 */
struct test
{
    float value;     /* value to test the function with */
    float expected;  /* expected result */
    float variance;  /* maximum delta between the expected and actual result */
};

/**
 * validate
 *
 * test validation function
 */
void __cdecl validate(float value, float expected, float variance)
{
    float result = fabsf(value);

    /*
     * The test is valid when the difference between result
     * and expected is less than or equal to variance
     */
    float delta = fabsf(result - expected);

    if (delta > variance)
    {
        Fail("fabsf(%g) returned %10.9g when it should have returned %10.9g",
             value, result, expected);
    }
}

/**
 * validate
 *
 * test validation function for values returning NaN
 */
void __cdecl validate_isnan(float value)
{
    float result = fabsf(value);

    if (!_isnan(result))
    {
        Fail("fabsf(%g) returned %10.9g when it should have returned %10.9g",
             value, result, PAL_NAN);
    }
}

/**
 * main
 * 
 * executable entry point
 */
INT __cdecl main(INT argc, CHAR **argv)
{
    struct test tests[] = 
    {
        /* value            expected           variance */
        { PAL_NEGINF,       PAL_POSINF,        0 },
        { -3.14159265f,     3.14159265f,       PAL_EPSILON * 10 },    // value: -(pi)              expected:  pi
        { -2.71828183f,     2.71828183f,       PAL_EPSILON * 10 },    // value: -(e)               expected:  e
        { -2.30258509f,     2.30258509f,       PAL_EPSILON * 10 },    // value: -(ln(10))          expected:  ln(10)
        { -1.57079633f,     1.57079633f,       PAL_EPSILON * 10 },    // value: -(pi / 2)          expected:  pi / 2
        { -1.44269504f,     1.44269504f,       PAL_EPSILON * 10 },    // value: -(log2(e))         expected:  log2(e)
        { -1.41421356f,     1.41421356f,       PAL_EPSILON * 10 },    // value: -(sqrt(2))         expected:  sqrt(2)
        { -1.12837917f,     1.12837917f,       PAL_EPSILON * 10 },    // value: -(2 / sqrt(pi))    expected:  2 / sqrt(pi)
        { -1,               1,                 PAL_EPSILON * 10 },
        { -0.785398163f,    0.785398163f,      PAL_EPSILON },         // value: -(pi / 4)          expected:  pi / 4
        { -0.707106781f,    0.707106781f,      PAL_EPSILON },         // value: -(1 / sqrt(2))     expected:  1 / sqrt(2)
        { -0.693147181f,    0.693147181f,      PAL_EPSILON },         // value: -(ln(2))           expected:  ln(2)
        { -0.636619772f,    0.636619772f,      PAL_EPSILON },         // value: -(2 / pi)          expected:  2 / pi
        { -0.434294482f,    0.434294482f,      PAL_EPSILON },         // value: -(log10(e))        expected:  log10(e)
        { -0.318309886f,    0.318309886f,      PAL_EPSILON },         // value: -(1 / pi)          expected:  1 / pi
        { -0.0f,            0,                 PAL_EPSILON },
    };


    // PAL initialization
    if (PAL_Initialize(argc, argv) != 0)
    {
        return FAIL;
    }

    for (int i = 0; i < (sizeof(tests) / sizeof(struct test)); i++)
    {
        validate( tests[i].value, tests[i].expected, tests[i].variance);
        validate(-tests[i].value, tests[i].expected, tests[i].variance);
    }
    
    validate_isnan(PAL_NAN);

    PAL_Terminate();
    return PASS;
}
