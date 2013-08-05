#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include "MetropolisIsing.h"

int main (int argc, char * const argv[]) {
    
    std::string param_line;
    std::ifstream param_file ("params");
    
    if (param_file.is_open())
    {
        while ( param_file.good() )
        {
            std::getline(param_file,param_line);
        }
        param_file.close();
    }
    else std::cout << "Unable to open file ";
    
    int flag;
    opterr = 0;
    bool doT = false;
    bool doh = false;
    bool doSpeed = false;
    bool states_out = false;
    
    while ((flag = getopt (argc, argv, "ThsS")) != -1) {
        switch (flag) {
            case 'T':
                doT = true;
                break;
            case 'h':
                doh = true;
                break;
            case 's':
                states_out = true;
                break;
            case 'S':
                doSpeed = true;
                break;
            default:
                break;
        }
    }
    
    if (doT) {
            
        int start = 0;
        size_t found = param_line.find_first_of(" ");

        int n;
        std::stringstream(param_line.substr(start, found)) >> n;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        float h;
        std::stringstream(param_line.substr(start, found)) >> h;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        int range;
        std::stringstream(param_line.substr(start, found)) >> range;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        int sweeps;
        std::stringstream(param_line.substr(start, found)) >> sweeps;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        float kbT_min;
        std::stringstream(param_line.substr(start, found)) >> kbT_min;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        float kbT_max;
        std::stringstream(param_line.substr(start, found)) >> kbT_max;

        start = found + 1;
        found = param_line.find_first_of(" ",start);

        float kbT_step;
        std::stringstream(param_line.substr(start, found)) >> kbT_step;

        std::cout << "n:" << n << " h:" << h << " range:" << range << " sweeps:" << sweeps << " kbT_min:" << kbT_min << " kbT_max:" << kbT_max << " kbT_step:" << kbT_step << " output states:" << states_out  <<std::endl;

        //sweeps = 100;

        time_t start_run,end_run,start_temp,end_temp;
        double time_diff;

        time (&start_run);

        for (int i = 0; i < (kbT_max - kbT_min)/kbT_step + 1; i++) {
        //for (int i = 5; i < 6; i++) {
            time (&start_temp);
            
            float kbT = kbT_min + kbT_step * i;
            
            std::cout << "Temp : " << kbT << std::endl;
            
            MetropolisIsing * MI;
            
            MI = new MetropolisIsing;
            
            MI->initSim(n,kbT,h,1.0,range,true);
            
            char em_file_name[50];
            char state_file_name[50];
            
            sprintf(em_file_name, "isingEM_%.1f_%.1f_%i.dat",kbT,h,range);
            sprintf(state_file_name, "isingState_%.1f_%.1f_%i.dat",kbT,h,range);
            
            /*
            std::stringstream em_file_name_stream;
            em_file_name_stream << "isingEM_" << kbT << "_" << h << "_" << range << ".dat"; 
            std::stringstream state_file_name_stream;
            state_file_name_stream << "isingState_" << kbT << "_" << h << "_" << range << ".dat";
            
            std::string em_file_name = em_file_name_stream.str();
            std::string state_file_name = state_file_name_stream.str();
            
            //std::cout << em_file_name.str() << std::endl;
            std::ofstream em_file (em_file_name.data());
            std::ofstream state_file (state_file_name.data());
            */
            
            std::ofstream em_file (em_file_name);
            std::ofstream state_file;
            if (states_out)
                std::ofstream state_file (state_file_name);
            
            int sweep = 0;
            int try_num = 0;
            
            while (sweep < sweeps) {
                MI->try_flip();
                
                if (try_num%MI->get_N() == 0) {
                    if (em_file.is_open())
                    {
                        em_file << sweep << " " << MI->get_E() << " " << MI->get_M() << std::endl;
                    }
                    if (states_out) {
                        if (state_file.is_open())
                        {
                            for (int p = 0; p < MI->get_N(); p++) {
                                state_file << MI->get_state(p) << " ";
                            }
                            
                            state_file << std::endl;
                        }
                    }
                    
                    if ( sweep % 1000 == 0 )
                        std::cout << "    sweep " << sweep << " of " << sweeps << std::endl;

                    sweep++;
                }
                
                try_num++;
            }
            
            delete MI;
            
            time (&end_temp);
            time_diff = difftime (end_temp,start_temp);
            std::cout << "    Temp : " << kbT << " took " << time_diff << " seconds" << std::endl;
        }

        time (&end_run);
        time_diff = difftime (end_run,start_run);

        std::cout << "Total time: " << time_diff << " seconds" << std::endl;
        }
    else if (doh) {
        int start = 0;
        size_t found = param_line.find_first_of(" ");
        
        int n;
        std::stringstream(param_line.substr(start, found)) >> n;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        float h_f_min;
        std::stringstream(param_line.substr(start, found)) >> h_f_min;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        float h_f_max;
        std::stringstream(param_line.substr(start, found)) >> h_f_max;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        float h_f_step;
        std::stringstream(param_line.substr(start, found)) >> h_f_step;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        int range;
        std::stringstream(param_line.substr(start, found)) >> range;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        int eq_sweeps;
        std::stringstream(param_line.substr(start, found)) >> eq_sweeps;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        int sweeps;
        std::stringstream(param_line.substr(start, found)) >> sweeps;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        float kbT;
        std::stringstream(param_line.substr(start, found)) >> kbT;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        int iteration_start;
        std::stringstream(param_line.substr(start, found)) >> iteration_start;
        
        start = found + 1;
        found = param_line.find_first_of(" ",start);
        
        int iteration_end;
        std::stringstream(param_line.substr(start, found)) >> iteration_end;
        
        std::cout << "n:" << n << " h_f:" << h_f_min << " h_f_max:" << h_f_max << " h_f_step:" << h_f_step << " range:" << range << " eq_sweeps:" << eq_sweeps << " kbT:" << kbT << " iteration_start:" << iteration_start << " iteration_end:" << iteration_end << " output states:" << states_out  <<std::endl;
        
        //sweeps = 100;
        
        time_t start_run,end_run,start_h_f,end_h_f;
        double time_diff;
        float h_init = -10.0;
        
        time (&start_run);
        
        // loop over all h final fields
        for (int p = 0; p < (h_f_max - h_f_min)/h_f_step + 1; p++) {
            
            time (&start_h_f);
            
            float h_f = h_f_min + h_f_step * p;
            
            std::cout << "h final " << h_f << std::endl;
            
            // do iterations for current h final
            for (int i = 0; i < iteration_end - iteration_start; i++) {
                //for (int i = 5; i < 6; i++) {
                
                
                //float kbT = kbT_min + kbT_step * i;
                
                
                std::cout << "    Iteration " << iteration_start + i << " of " << iteration_end - 1 << std::endl;
                
                MetropolisIsing * MI;
                
                MI = new MetropolisIsing;
                
                MI->initSim(n,kbT,h_init,1.0,range,false);
                
                char em_file_name[50];
                char state_file_name[50];
                
                sprintf(em_file_name, "isingEM_%#f_%#f_%i_%i.dat",kbT,h_f,range,iteration_start + i);
                sprintf(state_file_name, "isingState_%#f_%#f_%i_%i.dat",kbT,h_f,range,iteration_start + i);
                /*
                 std::stringstream em_file_name_stream;
                 em_file_name_stream << "isingEM_" << kbT << "_" << h << "_" << range << ".dat"; 
                 std::stringstream state_file_name_stream;
                 state_file_name_stream << "isingState_" << kbT << "_" << h << "_" << range << ".dat";
                 
                 std::string em_file_name = em_file_name_stream.str();
                 std::string state_file_name = state_file_name_stream.str();
                 
                 //std::cout << em_file_name.str() << std::endl;
                 std::ofstream em_file (em_file_name.data());
                 std::ofstream state_file (state_file_name.data());
                 */
                
                std::ofstream em_file (em_file_name);
                std::ofstream state_file;
                if (states_out)
                    std::ofstream state_file (state_file_name);            
                
                int sweep = 0;
                int try_num = 0;
                bool flip_done = false;
                
                while (sweep < sweeps) {
                    MI->try_flip();
                    
                    if (try_num%MI->get_N() == 0) {
                        if (em_file.is_open())
                        {
                            em_file << sweep << " " << MI->get_E() << " " << MI->get_M() << std::endl;
                        }
                        if (states_out) {
                        if (state_file.is_open())
                            {
                                for (int p = 0; p < MI->get_N(); p++) {
                                    state_file << MI->get_state(p) << " ";
                                }
                                
                                state_file << std::endl;
                            }
                        }
                        
                        if ( sweep % 1000 == 0 )
                            std::cout << "        sweep " << sweep << " of " << sweeps << std::endl;
                        
                        sweep++;
                    }
                    
                    if (sweep == eq_sweeps && !flip_done) {
                        MI->set_h(h_f);
                        std::cout << "    swapping field to " << h_f << " at sweep " << sweep << std::endl;
                        flip_done = true;
                    }
                    
                    try_num++;
                }
                
                delete MI;
                
            }
            
            time (&end_h_f);
            time_diff = difftime (end_h_f,start_h_f);
            std::cout << "    h final " << h_f << " took " << time_diff << " seconds" << std::endl;
        }
        
        time (&end_run);
        time_diff = difftime (end_run,start_run);
        
        std::cout << "Total time: " << time_diff << " seconds" << std::endl;
        
    }
    else if (doSpeed) {
        
        MetropolisIsing * MI;
        
        int n = 100;
        float h = 0.0;
        float kbT = 1.0;
        //int range = 10;
        
        int sweeps = 100;
        int sweep;
        int try_num;
        
        int iter_num = 100;
        int range_num = 15;
        
        //float exp_time;
        //float dir_time;
        
        clock_t start_clock, end_clock;
        
        float out_exp_times[range_num][iter_num];
        float out_dir_times[range_num][iter_num];
        
        // test with inline calced exps
        
        std::cout << "start exp calc"<<std::endl;
        
        for (int r = 0; r<range_num; r++) {
            
            std::cout << "    range: " << r  << " of " << range_num - 1 <<std::endl;
            
            MI = new MetropolisIsing;
            
            MI->initSim(n,kbT,h,1.0,r,true);
            
            for (int iter = 0; iter <iter_num; iter++) {
                
                if ( iter % 10 == 0 )
                    std::cout << "        iter: " << iter  << " of " << iter_num - 1 <<std::endl;
                
                sweep = 0;
                try_num = 0;
                
                start_clock  = clock ();
                
                while (sweep < sweeps) {
                    MI->try_flip_exp();
                    
                    if (try_num%MI->get_N() == 0) {
                        sweep++;
                    }
                    
                    try_num++;
                }
                
                end_clock  = clock ();
                
                out_exp_times[r][iter] = float((end_clock-start_clock))/float(CLOCKS_PER_SEC);
            
            }
            //std::cout << "exp calc took " << exp_time  << " secs or " << sweeps/exp_time << " sweeps per sec" <<std::endl;
            
            delete MI;
        }
        
        
        // test with pre calced exps
        
        std::cout << "start direct calc"<<std::endl;
        
        for (int r = 0; r<range_num; r++) {
            
            std::cout << "    range: " << r  << " of " << range_num - 1 <<std::endl;
            
            MI = new MetropolisIsing;
            
            MI->initSim(n,kbT,h,1.0,r,true);
            
            for (int iter = 0; iter <iter_num; iter++) {
                
                if ( iter % 10 == 0 )
                    std::cout << "        iter: " << iter  << " of " << iter_num - 1 <<std::endl;
        
                sweep = 0;
                try_num = 0;
                
                start_clock  = clock ();
                
                while (sweep < sweeps) {
                    MI->try_flip();
                    
                    if (try_num%MI->get_N() == 0) {
                        sweep++;
                    }
                    
                    try_num++;
                }
                
                end_clock  = clock ();
                
                out_dir_times[r][iter] = float((end_clock-start_clock))/float(CLOCKS_PER_SEC);
            }
        
            //std::cout << "direct access took " << dir_time  << " secs or " << sweeps/dir_time << " sweeps per sec" <<std::endl;
            
            delete MI;
            
            //std::cout << "ratio " << dir_time/exp_time <<std::endl;
        }
        
        std::cout << "-- Exp calc times --"<<std::endl;
        
        for (int iter = 0; iter <iter_num; iter++) {
            for (int r = 0; r<range_num; r++) {
                std::cout << out_exp_times[r][iter] << ",";
            }
            std::cout << std::endl;
        }
        
        std::cout << "-- Dir access times --"<<std::endl;
        
        for (int iter = 0; iter <iter_num; iter++) {
            for (int r = 0; r<range_num; r++) {
                std::cout << out_dir_times[r][iter] << ",";
            }
            std::cout << std::endl;
        }
        
        
        
    
    }
    return 0;
}
