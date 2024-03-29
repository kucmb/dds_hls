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
#include <algorithm>
#include <fstream>


void writeToOutputFile(std::ofstream& outputFile,
					   hls::stream<data_tot>& outFifo_I,
					   hls::stream<data_tot>& outFifo_Q)
{
	data_tot tmp_i;
	data_tot tmp_q;
	while ((!outFifo_I.empty()) & (!outFifo_Q.empty()))
	{
		outFifo_I.read(tmp_i);
		outFifo_Q.read(tmp_q);

		for(int i = 0; i < N_CH; i++){
			outputFile << tmp_i((i+1)*BW_DATA-1, i*BW_DATA) << " ";
			outputFile << tmp_q((i+1)*BW_DATA-1, i*BW_DATA) << " ";
		}
		outputFile << std::endl;
	}
}

int main(int argc, char* argv[]) {
	hls::stream<data_tot> outFifo_I("outFIFO_I");
	hls::stream<data_tot> outFifo_Q("outFIFO_Q");

	const int length = 100;
	const ph pinc = 256;

    std::string currentDirectory = "/home/jsuzuki/fpga_projects/dds_hls/";

	std::ifstream inputFile;
	std::ofstream outputFile;
	outputFile.open(currentDirectory+"/test.out");

	for(int i = 0; i < length; i++){
		dds_hls(pinc, outFifo_I, outFifo_Q);
	}

	writeToOutputFile(outputFile, outFifo_I, outFifo_Q);

	outputFile.close();
	
	return 0;
}
