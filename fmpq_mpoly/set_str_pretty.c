/*
    Copyright (C) 2019-2021 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include "fmpq_mpoly.h"

int fmpq_mpoly_set_str_pretty(fmpq_mpoly_t poly, const char * str,
                                    const char** x, const fmpq_mpoly_ctx_t ctx)
{
    int ret;
    slong i;
    fmpq_mpoly_t val;
    fparse_t E;
    char dummy[FLINT_BITS/2];

    fparse_init(E, (void (*)(void *, const void *)) fmpq_mpoly_init,
                             sizeof(fmpq_mpoly_struct), (const void *) ctx);

    E->clear_fxn = (void (*)(void *, const void *)) fmpq_mpoly_clear;
    E->swap_fxn = (void (*)(void *, void *, const void *)) fmpq_mpoly_swap;
    E->set_fxn = (void (*)(void *, const void *, const void *)) fmpq_mpoly_set;
    E->set_fmpz_fxn = (void (*)(void *, const fmpz_t, const void *)) fmpq_mpoly_set_fmpz;
    E->pow_fmpz_fxn = (int (*)(void *, const void *, const fmpz_t, const void *)) fmpq_mpoly_pow_fmpz;
    E->mul_fxn = (void (*)(void *, const void *, const void *, const void *)) fmpq_mpoly_mul;
    E->add_fxn = (void (*)(void *, const void *, const void *, const void *)) fmpq_mpoly_add;
    E->sub_fxn = (void (*)(void *, const void *, const void *, const void *)) fmpq_mpoly_sub;
    E->neg_fxn = (void (*)(void *, const void *, const void *)) fmpq_mpoly_neg;
    E->div_fxn = (int (*)(void *, const void *, const void *, const void *)) fmpq_mpoly_divides;
    E->length_fxn = (slong (*)(const void *, const void *)) fmpq_mpoly_length;

    fmpq_mpoly_init(val, ctx);
    for (i = 0; i < ctx->zctx->minfo->nvars; i++)
    {
        fmpq_mpoly_gen(val, i, ctx);
        if (x == NULL)
        {
            flint_sprintf(dummy, "x%wd", i + 1);
            fparse_add_terminal(E, dummy, (const void *)val);
        }
        else
        {
            fparse_add_terminal(E, x[i], (const void *)val);
        }
    }
    fmpq_mpoly_clear(val, ctx);

    ret = fparse_parse(E, poly, str, strlen(str));

    fparse_clear(E);

    return ret;
}

