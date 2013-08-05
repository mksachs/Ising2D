import numpy as np
import cPickle
import os
import sys
import getopt
import glob
import matplotlib as mp
import matplotlib.pyplot as plt
from scipy import fftpack
from scipy import integrate

def plot_e_m(sim_name,run_type):
    os.chdir(sim_name)
    
    EMFiles = glob.glob( os.path.join('isingEM*.dat') )
    
    if (run_type == "temp"): 

        def format_params(params):
            return int(params[0]), float(params[1]), int(params[2]), int(params[3]), float(params[4]), float(params[5]), float(params[6])

        f = open('params','r')
                
        for ln, line in enumerate(f):
            if ln == 1:
                n, h, range, sweeps, T_min, T_max, T_step = format_params(line.split())

        f.close()
        
        print 'n:%i h:%.1f range:%i sweeps:%i T_min:%.1f T_max:%.1f T_step:%.1f'%(n, h, range, sweeps, T_min, T_max, T_step)
        
        #T = np.arange(T_min, T_max + T_step, T_step)
        N = n**2.0

        for file in EMFiles:
            #file = glob.glob( os.path.join('isingEM_%.1f_*'%currT) )[0]
            
            m = []
            e = []
            
            f = open(file,'r')
            
            currT = float(file.split("_")[1])
            
            for line in f:
                sweep, E, M = map(float,line.split())
                e.append(E/N)
                m.append(M/N)
            
                #for k,v in dat.iteritems():
                #    print k,',',v[0],',',v[1]
            f.close()

            min_e = min(e)
            max_e = max(e)

            t = np.arange(0, len(e), 1)

            #print len(t),len(m),len(e)
            # axes rect in relative 0,1 coords left, bottom, width, height.  Turn
            # off xtick labels on all but the lower plot

            #print np.arange(min_e, max_e)

            fig = plt.figure()

            fig = plt.figure(figsize=[8.5, 11])

            plt.subplots_adjust(hspace=0.5)

            ax1 = plt.subplot(211)
            ax1.plot(t,m)
            plt.ylim(-1.1,1.1)

            ax2 = plt.subplot(212, sharex=ax1)
            ax2.plot(t,e)
            plt.ylim(min_e-0.1,max_e+0.1)

            ax1.set_xlabel('t')
            ax2.set_xlabel('t')

            ax1.set_ylabel('m')
            ax2.set_ylabel('e')

            ax1.set_title('Temp: %f'%currT)

            plt.savefig('isingMEplot-%#f.pdf'%currT, format='pdf')
        
    elif (run_type == "field"):
        
        def format_params(params):
            #      n               h_f_min           h_f_max         h_f_step            range           eq_sweeps         sweeps          T                 iterations
            return int(params[0]), float(params[1]), float(params[2]), float(params[3]), int(params[4]), float(params[5]), int(params[6]), float(params[7]), int(params[8])

        f = open('params','r')
                
        for ln, line in enumerate(f):
            if ln == 1:
                n, h_f_min, h_f_max, h_f_step, range, eq_sweeps, sweeps, T, iterations = format_params(line.split())

        f.close()
        
        print 'n:%i h_f_min:%.1f h_f_max:%.1f h_f_step:%.1f range:%i eq_sweeps:%i sweeps:%i T:%.1f iterations:%i'%(n, h_f_min, h_f_max, h_f_step, range, eq_sweeps, sweeps, T, iterations)
        
        #i_s = np.arange(iterations)
        N = n**2.0
        
        #h_fs = np.arange(h_f_min, h_f_max + h_f_step, h_f_step)
        
        for file in EMFiles:
        
            #files = glob.glob( os.path.join('isingEM_%#f_%#f_%i*'%(T,currh_f,range)) )
            
            #for file in files:
                #[0]
            
            m = []
            e = []
            
            currT = float(file.split("_")[1])
            currh_f = float(file.split("_")[2])
            range = int(file.split("_")[3])
            currIt = file.split("_")[4].split(".")[0]
            
            f = open(file,'r')
            
            for line in f:
                sweep, E, M = map(float,line.split())
                e.append(E/N)
                m.append(M/N)
            
                #for k,v in dat.iteritems():
                #    print k,',',v[0],',',v[1]
            f.close()

            min_e = min(e)
            max_e = max(e)

            t = np.arange(0, len(e), 1)

            #print len(t),len(m),len(e)
            # axes rect in relative 0,1 coords left, bottom, width, height.  Turn
            # off xtick labels on all but the lower plot

            #print np.arange(min_e, max_e)

            fig = plt.figure()

            fig = plt.figure(figsize=[8.5, 11])

            plt.subplots_adjust(hspace=0.5)

            ax1 = plt.subplot(211)
            ax1.plot(t,m)
            plt.ylim(-1.1,1.1)

            ax2 = plt.subplot(212, sharex=ax1)
            ax2.plot(t,e)
            plt.ylim(min_e-0.1,max_e+0.1)

            ax1.set_xlabel('t')
            ax2.set_xlabel('t')

            ax1.set_ylabel('m')
            ax2.set_ylabel('e')

            ax1.set_title(file)

            plt.savefig('isingMEplot_%#f_%#f_%i_%s.pdf'%(currT,currh_f,range,currIt), format='pdf')

def find_corr_time(sim_name):
    os.chdir(sim_name)
    
    EMFiles = glob.glob( os.path.join('isingEM*.dat') )
    
    kbTs = []
    
    for file in EMFiles:
        kbTs.append(float(file.split("_")[1]))
    
    try:
        f = open('therm','r')
        thermSweeps = {}
        for line in f:
            kbT, tSweep = line.split()
            thermSweeps[float(kbT)] = int(tSweep)
        f.close()
    except IOError:
        thermSweeps = get_therm_vals(kbTs)
        
    def format_params(params):
        return int(params[0]), float(params[1]), int(params[2]), int(params[3]), float(params[4]), float(params[5]), float(params[6])

    f = open('params','r')
            
    for ln, line in enumerate(f):
        if ln == 1:
            n, h, range, sweeps, T_min, T_max, T_step = format_params(line.split())

    f.close()
    
    print 'n:%i h:%.1f range:%i sweeps:%i T_min:%.1f T_max:%.1f'%(n, h, range, sweeps, kbTs[0], kbTs[-1])

    
    #T = np.arange(T_min, T_max + T_step, T_step)
    N = n**2.0
    
    chis = []
    
    for fn, file in enumerate(EMFiles):
    
        #if fn == 2 or fn == 6:
        
        currT = float(file.split("_")[1])
        
        thermSweep = thermSweeps[currT]

        m = []

        f = open(file,'r')
    
        for ln,line in enumerate(f):
            if ln >= thermSweep:
                sweep, E, M = map(float,line.split())
                m.append(M/N)
        
            #for k,v in dat.iteritems():
            #    print k,',',v[0],',',v[1]
        f.close()
                    
        m_np = np.asarray(m)
        
        print 'temp: ',currT
        '''
        phi_m = []
        
        for t, m_t in enumerate(m_np):
            phi_m.append( (np.mean(m_np[0]*m_np[0:t]) - m_sq_mean)/(np.mean(m_np[0]**2.0) - m_sq_mean))
        '''
        
        m_sq_mean = np.mean(m_np[0:1000])**2.0
        m_mean_sq = np.mean(m_np[0:1000]**2.0)
        chi = []
        t_max = len(m_np)
        #m_mean_sq = np.mean(m_np[0:1000])**2.0
        
        for t, m_t in enumerate(m_np[0:1000]):
            if (t%100 == 0):
                print "    ",t
            #print "    ",t
            norm_fac = 1.0/(t_max - t)
            term1 = 0.0
            term2 = 0.0
            term3 = 0.0
            for tp in np.arange(0,t_max - t):
                term1 += m_np[tp]*m_np[tp+t]
                term2 += m_np[tp]
                term3 += m_np[tp+t]
            chi.append((1/(m_mean_sq-m_sq_mean))*(norm_fac*term1 - norm_fac*term2*norm_fac*term3))
        
        chis.append(chi/max(chi))
        
    cols = len(chis)
    rows = len(chis[0])

    for r in np.arange(rows):
        row_out = ''
        for c in np.arange(cols):
            row_out += '%f,'%chis[c][r]
        print row_out

def get_therm_vals(kbTs):
    thermSweeps = {}
    
    f = open('therm','w')
    
    for kbT in kbTs:
        thermSweeps[kbT] = int(raw_input("Therm sweep for temp %f: "%kbT))
        f.write("%f %i\n"%(kbT,thermSweeps[kbT]))
    
    f.close()
    
    return thermSweeps
    
def find_c_chi_m(sim_name):
    os.chdir(sim_name)
    
    EMFiles = glob.glob( os.path.join('isingEM*.dat') )
    
    kbTs = []
    
    for file in EMFiles:
        kbTs.append(float(file.split("_")[1]))
    
    try:
        f = open('therm','r')
        thermSweeps = {}
        for line in f:
            kbT, tSweep = line.split()
            thermSweeps[float(kbT)] = int(tSweep)
        f.close()
    except IOError:
        thermSweeps = get_therm_vals(kbTs)
        
    def format_params(params):
        return int(params[0]), float(params[1]), int(params[2]), int(params[3]), float(params[4]), float(params[5]), float(params[6])

    f = open('params','r')
            
    for ln, line in enumerate(f):
        if ln == 1:
            n, h, range, sweeps, T_min, T_max, T_step = format_params(line.split())

    f.close()
    
    print 'n:%i h:%.1f range:%i sweeps:%i T_min:%.1f T_max:%.1f'%(n, h, range, sweeps, kbTs[0], kbTs[-1])

    
    #T = np.arange(T_min, T_max + T_step, T_step)
    N = n**2.0
    
    for file in EMFiles:
        
        currT = float(file.split("_")[1])
        
        thermSweep = thermSweeps[currT]
        
        #file = glob.glob( os.path.join('isingEM_%.1f_*'%currT) )[0]

        m = []
        Es = []

        f = open(file,'r')
    
        for ln,line in enumerate(f):
            if ln >= thermSweep:
                sweep, E, M = map(float,line.split())
                Es.append(E)
                m.append(M/N)
        
            #for k,v in dat.iteritems():
            #    print k,',',v[0],',',v[1]
        f.close()
                    
        m_np = np.asarray(m)
        E_np = np.asarray(Es)
        
        m_sq_mean = np.mean(m_np)**2.0
        m_mean_sq = np.mean(m_np**2.0)
        
        E_sq_mean = np.mean(E_np)**2.0
        E_mean_sq = np.mean(E_np**2.0)
        
        chi = (N/currT)*(m_mean_sq-m_sq_mean)
        c = (1/((currT**2)*N))*(E_mean_sq-E_sq_mean)
        m_mean = abs(np.mean(m_np))
        
        print currT,',',chi,',',c,',',m_mean

def corr_length(sim_name):
    os.chdir(sim_name)
    
    EMFiles = glob.glob( os.path.join('isingEM*.dat') )
    stateFiles = glob.glob( os.path.join('isingState*.dat') )
    
    if len(stateFiles) == 0:
        print "Need state files for this calculation!"
        return
    
    kbTs = []
    
    for file in EMFiles:
        kbTs.append(float(file.split("_")[1]))
    
    try:
        f = open('therm','r')
        thermSweeps = {}
        for line in f:
            kbT, tSweep = line.split()
            thermSweeps[float(kbT)] = int(tSweep)
        f.close()
    except IOError:
        thermSweeps = get_therm_vals(kbTs)
        
    def format_params(params):
        return int(params[0]), float(params[1]), int(params[2]), int(params[3]), float(params[4]), float(params[5]), float(params[6])

    f = open('params','r')
            
    for ln, line in enumerate(f):
        if ln == 1:
            n, h, range, sweeps, T_min, T_max, T_step = format_params(line.split())

    f.close()
    
    print 'n:%i h:%.1f range:%i sweeps:%i T_min:%.1f T_max:%.1f'%(n, h, range, sweeps, kbTs[0], kbTs[-1])

    
    #T = np.arange(T_min, T_max + T_step, T_step)
    N = n**2.0
    
    for e_m_file in EMFiles:
        
        currT = float(e_m_file.split("_")[1])
        
        thermSweep = thermSweeps[currT]            
        state_file = glob.glob( os.path.join('isingState_%.1f_*'%currT) )[0]
        
        m = []

        f = open(e_m_file,'r')
    
        for ln,line in enumerate(f):
            if ln >= thermSweep:
                sweep, E, M = map(float,line.split())
                m.append(M/N)
        
            #for k,v in dat.iteritems():
            #    print k,',',v[0],',',v[1]
        f.close()
                    
        m_np = np.asarray(m)
        
        m_mean = np.mean(m_np)
        
        f = open(state_file,'r')
        
        iterations = 0
        
        corrs = np.zeros(n)
        
        print 'temp: ',currT
        
        for ln,line in enumerate(f):
            if ln >= thermSweep and ln <= thermSweep + 1000:
                state_in = map(int,line.split())
                state_np_flat = np.asarray(state_in)
                #print state_np
                state_np = np.reshape(state_np_flat,(n,n))
                sp = state_np - m_mean
                st = np.absolute(fftpack.fft(sp))
                stsq = st**2
                s = fftpack.ifft(stsq).astype(float)
                
                corrs += np.sum(s,axis=0)/(n)
                
                if (iterations%100 == 0):
                    print "    ",iterations
                
                iterations += 1
                
        f.close()
        
        '''
        for r in np.arange(n):
            r_p = ''
            for c in np.arange(n):
                if state_np[r,c] < 0:
                    r_p += '%i '%state_np[r][c]
                if state_np[r,c] > 0:
                    r_p += ' %i '%state_np[r][c]
            print r_p
        '''
        
        for r, corr in enumerate(corrs):
            print r,',',corr/iterations
        print
        print
            
def domain_scaling(sim_name,kbT):
    os.chdir(sim_name)

    def format_params(params):
        return int(params[0]), float(params[1]), int(params[2]), float(params[3]), float(params[4]), float(params[5])

    f = open('params','r')
            
    for ln, line in enumerate(f):
        if ln == 1:
            n, h, sweeps, T_min, T_max, T_step = format_params(line.split())

    f.close()
    
    N = n**2.0

def benchmarking(sim_name):
    os.chdir(sim_name)
    
    f_exp = open('exp_benchmarks','r')
    f_dir = open('dir_benchmarks','r')
    
    exp_vals = [];
    dir_vals = [];
    
    sweeps = 10.0;
    
    for line in f_exp:
        #print line.split(',').pop()
        exp_vals.append( map(float, line.split(',')) )
        #print exp_vals[-1]
    
    for line in f_dir:
        dir_vals.append( map(float, line.split(',')) )
        #print dir_vals[-1]
    
    dir_vals_np = np.asarray(dir_vals)
    exp_vals_np = np.asarray(exp_vals)
    
    #print dir_vals_np.T
    
    print '--- Exp calc ---'
    for r, dat in enumerate(exp_vals_np.T):
        print r,',',(sweeps/dat).mean(),',' ,(sweeps/dat).std()
        
    print '--- Dir access ---'
    for r, dat in enumerate(dir_vals_np.T):
        print r,',',(sweeps/dat).mean(),',' ,(sweeps/dat).std()
        
    f_exp.close()
    f_dir.close()
    
    
    
class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

def main(argv=None):
    if argv is None:
        argv = sys.argv
    try:
        try:
            opts, args = getopt.getopt(argv[1:], 'Tha:')
        except getopt.error, msg:
            raise Usage(msg)
        
        sim_name = args[0]
        run_type = ""
        
        for o in opts:
            if o[0] == '-a':
                action = o[1]
            elif o[0] == '-T':
                run_type = "temp"
            elif o[0] == '-h':
                run_type = "field"
            
        
        print action
        
        if action == 'plot_e_m':
            plot_e_m(sim_name, run_type)
        elif action == 'find_corr_time':
            find_corr_time(sim_name)
        elif action == 'find_c_chi_m':
            find_c_chi_m(sim_name)
        elif action == 'corr_length':
            corr_length(sim_name)
        elif action == 'benchmarking':
            benchmarking(sim_name)
            
    
    except Usage, err:
        print >>sys.stderr, sys.argv[0].split('/')[-1] + ': ' + str(err.msg)
        print >>sys.stderr, '   for help use --help'
        return
                
if __name__ == '__main__': 
    sys.exit(main())

    