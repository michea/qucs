function pl = createPulseNDataSet (pw, per, qvect)

% Create the contents of Qucs pulse source data set to the local data structure

% -*- texinfo -*-
% @deftypefn {Function File} createPulseNDataSet (@var{pw} @var{per}, @var{qvect})
% Parses the content of the pulse vector set @var{qvect} which contains Number of pulse data vectors.
% @end deftypefn

% Copyright 2017 (C) 2017 by Michael Arendall <miche.arendall@gmail.com`>
% Published under GNU General Public License (GPL V3). No warranty at all.
%
%

% local variables
%
time = qvect(1);    % qvect(1) is TIME vector (indep) sim time from .TRAN
num  = qvect(2);    % qvect(2) is Number (indep) number of .TRAN sims
iPr1 = qvect(3);    % qvect(3) - qvect(Number+2) start of data vectors I(PR1), V(VIN), V(VMR), V(VOUT)
vVin = num.len;
vVmr = num.len*2;
vVout = num.len*3;
%
% pulse parameters
pwidth = pw;
period = per;
tick = pwidth/2;
%
% data vectors
tmls = [];

t = 1;  % time sample no.
pc = 1; % pulse count based on period based on (1/2) pulse width + period

% build current vs. pulse incremental reponse
for j = 3:num.len+2
    for i = 1:time.len
        % sample points at specified time intervals
        if time.data(i) > tick;
            tmls(i) = time.data(i);
            
            % increment pulse count and copy memristor current value iMrm
            tstep(t) = tick;
            pl(pc).no = pc;
            pl(pc).iMrm = qvect(j).data(i);
            pl(pc).vMrm = qvect(j+vVmr).data(i);
            pl(pc).rMrm = pl(pc).vMrm/pl(pc).iMrm;

            % debug
            %fprintf(1,'\npulse = %d j = %d t = %d tstep = %g iMrm = %g vMrm = %g rMrm = %g\n',pl(pc).no, j, t, tstep(t), pl(pc).iMrm, pl(pc).vMrm, pl(pc).rMrm);
            
            pc = pc + 1;    
            t = t + 1;
            tick = tick + period;
        end
    end
    t = 1;
    tick = pwidth/2;
end

endfunction