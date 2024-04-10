/*
 * Copyright (c) 2023, KUHEP, Kyoto University
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

#include "dds_hls.hpp"

const ph phase_offset_q = (1 << (BW_PHASE - 2));
const ph phase_offset_i = 0;

data calc_i(ph phase){
    #pragma HLS inline recursive
    ph_f phase_pi;
    //ph_f offset = 0.5;
    // Bitwize interpretation
    phase_pi(BW_PHASE-1, 0) = phase(BW_PHASE-1, 0);

    return hls::cospi(phase_pi);
    //return hls::cospi(phase_pi - offset);
}

data calc_q(ph phase){
    #pragma HLS inline recursive
    ph_f phase_pi;
    //ph_f offset = 0.5;
    // Bitwize interpretation
    phase_pi(BW_PHASE-1, 0) = phase(BW_PHASE-1, 0);

    //return hls::cospi(phase_pi - offset);
    return hls::cospi(phase_pi);
}

void calc_iq(ph pinc,
             ph phase_i,
             ph phase_q,
             ph stage,
             data& data_i,
             data& data_q)
{
    #pragma HLS PIPELINE II=1
	ph phase_now;
    data i_tmp = calc_i(phase_i + pinc*stage);
    data q_tmp = calc_q(phase_q + pinc*stage);

    data_i = i_tmp;
    data_q = q_tmp;
}

void push(data buffer[N_CH], hls::stream<data_tot>& out){
    #pragma HLS PIPELINE II=1
    data_tot tmp;
    for(int i = 0; i < N_CH; i++){
        tmp(BW_DATA*(i+1)-1, BW_DATA*i) = buffer[i](BW_DATA-1,0);
    }
    out.write(tmp);
}

void phase_calc(ph pinc,
                ph& phase_i,
                ph& phase_q){
    #pragma HLS PIPELINE II=1
    static ph phase_now = 0;

    phase_i = phase_now + phase_offset_i;
    phase_q = phase_now + phase_offset_q;
    phase_now += pinc*N_CH;
}

void dds_hls(ph pinc,
             hls::stream<data_tot>& data_i,
             hls::stream<data_tot>& data_q){
// Input
#pragma HLS INTERFACE mode=s_axilite port=pinc clock=s_axi_aclk
// Output
#pragma HLS INTERFACE axis register port=data_i name=m_axis_data_i
#pragma HLS INTERFACE axis register port=data_q name=m_axis_data_q

// Ctrl interface suppression.
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS DATAFLOW
#pragma HLS stable variable=pinc
    
    //static hls::stream<ph,100> phase_int;
    static ph phase_tmp_i, phase_tmp_q;
    static data i_buffer[N_CH];
    static data q_buffer[N_CH];

    phase_calc(pinc, phase_tmp_i, phase_tmp_q);

    calc_loop: for(int i = 0; i < N_CH; i++){
        #pragma HLS unroll
        calc_iq(pinc, phase_tmp_i, phase_tmp_q, i, i_buffer[i], q_buffer[i]);
    }
    push(i_buffer, data_i);
    push(q_buffer, data_q);
}
