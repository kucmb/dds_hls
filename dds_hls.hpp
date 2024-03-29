/*
 * Copyright (c) 2023, KUHEP, Kyoto University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef DDS_HLS
#define DDS_HLS

#include <hls_math.h>
#include <hls_stream.h>
#include <ap_int.h>

#define BW_PHASE 16
#define BW_DATA 16


const int N_CH = 16;
ap_uint<16> calc_i(ap_uint<BW_PHASE> phase, ap_uint<BW_PHASE> pinc, ap_uint<BW_PHASE> stage);
ap_uint<16> calc_q(ap_uint<BW_PHASE> phase, ap_uint<BW_PHASE> pinc, ap_uint<BW_PHASE> stage);

typedef ap_uint<BW_PHASE> ph;
typedef ap_fixed<BW_PHASE,1> ph_f;
typedef ap_fixed<BW_DATA,1, AP_TRN, AP_SAT> data;
typedef ap_uint<BW_DATA*N_CH> data_tot;

data calc_i(ph phase);
data calc_q(ph phase);

void calc_iq(ph pinc,
             ph phase,
             ph stage,
             data& data_i,
             data& data_q);

void push(data buffer[N_CH], hls::stream<data_tot>& out);
void phase_calc(ph pinc, ph& phase);

void dds_hls(ph pinc,
             hls::stream<data_tot>& data_i,
             hls::stream<data_tot>& data_q);

#endif
