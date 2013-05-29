/**
 * Copyright (c) 2007-2012, Timothy Stack
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "data_parser.hh"

using namespace std;

data_format data_parser::FORMAT_SEMI(DT_COMMA, DT_SEMI);
data_format data_parser::FORMAT_COMMA(DT_INVALID, DT_COMMA);
data_format data_parser::FORMAT_PLAIN(DT_INVALID, DT_INVALID);

data_format_state_t dfs_semi_next(data_format_state_t state,
                                  data_token_t next_token)
{
    data_format_state_t retval = state;

    switch (state) {
    case DFS_INIT:
        switch (next_token) {
        case DT_COMMA:
        case DT_SEMI:
            retval = DFS_ERROR;
            break;

        default: retval = DFS_KEY; break;
        }
        break;

    case DFS_KEY:
        switch (next_token) {
        case DT_SEPARATOR: retval = DFS_VALUE; break;

        case DT_SEMI: retval = DFS_ERROR; break;

        default: break;
        }
        break;

    case DFS_VALUE:
        switch (next_token) {
        case DT_SEMI: retval = DFS_INIT; break;

        default: break;
        }
        break;

    case DFS_ERROR: retval = DFS_ERROR; break;
    }

    return retval;
}

data_format_state_t dfs_comma_next(data_format_state_t state,
                                   data_token_t next_token)
{
    data_format_state_t retval = state;

    switch (state) {
    case DFS_INIT:
        switch (next_token) {
        case DT_COMMA:
        case DT_SEMI:
            retval = DFS_ERROR;
            break;

        default:
            retval = DFS_KEY;
            break;
        }
        break;

    case DFS_KEY:
        switch (next_token) {
        case DT_SEPARATOR:
            retval = DFS_VALUE;
            break;

        case DT_SEMI:
        case DT_COMMA:
            retval = DFS_ERROR;
            break;

        default: break;
        }
        break;

    case DFS_VALUE:
        switch (next_token) {
        case DT_COMMA:
            retval = DFS_INIT;
            break;

        case DT_SEPARATOR:
            retval = DFS_VALUE;
            break;

        default: break;
        }
        break;

    case DFS_ERROR:
        retval = DFS_ERROR;
        break;
    }

    return retval;
}