/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011, 2012 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "long_extras.h"
#include "ulong_extras.h"
#include "padic_poly.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    padic_ctx_t ctx;
    fmpz_t p;
    slong N;

    printf("compose... ");
    fflush(stdout);

    flint_randinit(state);

    /* Compare with the computation over QQ */
    for (i = 0; i < 100; i++)
    {
        padic_poly_t f, g, h, h2;
        fmpq_poly_t fQQ, gQQ, hQQ;

        fmpz_init_set_ui(p, n_randtest_prime(state, 0));
        N = n_randint(state, PADIC_TEST_PREC_MAX - PADIC_TEST_PREC_MIN) 
            + PADIC_TEST_PREC_MIN;
        padic_ctx_init(ctx, p, FLINT_MAX(0, N-10), FLINT_MAX(0, N+10), PADIC_SERIES);

        padic_poly_init2(f,  0, N);
        padic_poly_init2(g,  0, N);
        padic_poly_init2(h,  0, N);
        padic_poly_init2(h2, 0, N);
        fmpq_poly_init(fQQ);
        fmpq_poly_init(gQQ);
        fmpq_poly_init(hQQ);

        padic_poly_randtest(f, state, n_randint(state, 40), ctx);
        padic_poly_randtest(g, state, n_randint(state, 15), ctx);

        padic_poly_get_fmpq_poly(fQQ, f, ctx);
        padic_poly_get_fmpq_poly(gQQ, g, ctx);

        padic_poly_compose(h, f, g, ctx);
        fmpq_poly_compose(hQQ, fQQ, gQQ);

        padic_poly_set_fmpq_poly(h2, hQQ, ctx);

        if (padic_poly_val(g) >= 0)
        {
            result = (padic_poly_equal(h, h2) && padic_poly_is_reduced(h, ctx));
            if (!result)
            {
                printf("FAIL (cmp with QQ, ord_p(g) >= 0):\n");
                printf("f  = "), padic_poly_print(f, ctx),  printf("\n\n");
                printf("g  = "), padic_poly_print(g, ctx),  printf("\n\n");
                printf("h  = "), padic_poly_debug(h),  printf("\n\n");
                printf("h2 = "), padic_poly_debug(h2), printf("\n\n");
                printf("p  = "), fmpz_print(p), printf("\n\n");
                printf("N  = %ld\n\n", N);
                abort();
            }
        }
        else
        {
            slong N2 = N + (f->length - 1) * padic_poly_val(g);
            padic_poly_t hX, h2X;

            padic_poly_init2(hX,  0, N2);
            padic_poly_init2(h2X, 0, N2);

            padic_poly_set(hX,  h,  ctx);
            padic_poly_set(h2X, h2, ctx);

            result = (padic_poly_equal(hX, h2X) && padic_poly_is_reduced(hX, ctx));
            if (!result)
            {
                printf("FAIL (cmp with QQ, ord_p(g) < 0):\n");
                printf("f   = "), padic_poly_print(f,   ctx), printf("\n\n");
                printf("g   = "), padic_poly_print(g,   ctx), printf("\n\n");
                printf("h   = "), padic_poly_print(h,   ctx), printf("\n\n");
                printf("h2  = "), padic_poly_print(h2,  ctx), printf("\n\n");
                printf("hX  = "), padic_poly_print(hX,  ctx), printf("\n\n");
                printf("h2X = "), padic_poly_print(h2X, ctx), printf("\n\n");
                printf("p   = "), fmpz_print(p), printf("\n\n");
                printf("N   = %ld\n\n", N);
                printf("N2  = %ld\n\n", N2);
                abort();
            }

            padic_poly_clear(hX);
            padic_poly_clear(h2X);
        }

        padic_poly_clear(f);
        padic_poly_clear(g);
        padic_poly_clear(h);
        padic_poly_clear(h2);
        fmpq_poly_clear(fQQ);
        fmpq_poly_clear(gQQ);
        fmpq_poly_clear(hQQ);

        padic_ctx_clear(ctx);
        fmpz_clear(p);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
