function dataSet = loadQucsNDataSet(dataSetFile)
% Load a dataset produced by the Qucs circuit simulator.
%
% Syntax
%
% dataSet = loadQucsNDataSet(dataSetFile)
%
% Input
%
%  dataSetFile - name of the data file produced by Qucs.
%  
%
% Output
%
%  dataSet - an array of structures containing the information from the
%    Qucs data file. Each structure contains the following fields:
%
%    name       | The name of the variable associated with the data in the 
%               | 'data' field of the structure, e.g. 
%               |
%    nameDep    | The name of the dependent variable associated with the
%               | data, e.g. for a time series simulation this will be
%               | 'time' and another structure will hold the time data as a
%               | variable.
%               | 
%    dep        | Value of 0 or 1 specifying if the data in the data field 
%               | is dependent on another variable. 
%               |
%    data       | Vector of values containing the data for the specified
%               | variable
%

% -*- texinfo -*-
% @deftypefn {Function File} {[@var{data}] =} loadQucsNDataSet (@var{filename})
% Loads the Qucs dataset from the Qucs dataset file @var{filename} containing N data
% sets specified by the indep variable Number.
% @end deftypefn

% Copyright 2010 (C) 2010 by Eberhard Gamm <eberhard.gamm@gmx.de>
% Modified  2017 Michael Arendall
% Originally published under GNU General Public License (GPL V2). No warranty at all.
% Modifications published under GNU General Public License (GPL v3). No warranty at all.
%
    dataSet = [];
    fid = fopen(dataSetFile,'r');
    if fid < 0
        fprintf(1,'Data set %s not found\n',dataSetFile);
	    return;
    end
    progress = "";
    pchar = '.';
    status = "";
    error = 0;
    idata = 0;
    idx = 0;
    num = 0;
    nset = 0;
    dset = 1;

    status = 'parsing data ';
    fprintf('%s\n', status);
    
    while ~feof(fid) && ~error
        line = fgetl(fid);
        if ~ischar(line)
            break;
        end
        if isempty(line)
            break;
        end
        ll = length(line);
        % process section
        if line(1) == '<'
            % process independent variable
            if strcmp(line(1:min(7,ll)),'<indep ')
                progress = strcat(progress, '.');
                fprintf(1, '%s', progress);
    
                out = textscan(line(8:end-1),'%s %d');
                if numel(out) ~= 2
                    fprintf(1,'Invalid independent variable in data set %s\n',dataSetFile);
                    error = 1;
                    break;
                end
                name = out{1}{1};
                nl = length(name);

                % check independent name - if Number then process N datasets
                if strcmp(name(1:min(6,nl)),'Number')
                  % preserve numdatapoints to be restored later - nset is number of data sets
                  nset = out{2};
                  num = numdatapoints;
                end
               
                numdatapoints = out{2};
                idx = idx + 1;
                dataSet(idx).name = name;
                dataSet(idx).nameDep = '-';
                dataSet(idx).dep = 0;
                dataSet(idx).data = zeros(1,numdatapoints);
                idata = 1;
            end
           % process dependent variable 
            if strcmp(line(1:min(5,ll)),'<dep ')
                progress = strcat(progress,'.');
                fprintf(1, '%s', progress);
                
                % pattern <dep name TIME Number>
                out = textscan(line(5:end-1),'%s %s %s');
                if numel(out) ~= 3
                    fprintf(1,'Invalid dependent variable - missing Number in data set %s\n',dataSetFile);
                    error = 1;
                    break;
                end

                % read Number so restore numdatapoints of <indep
                numdatapoints = num;

                % get variable name and dependencies
                name = out{1}{1};
                nameDep = out{3}{1};

                % increment vector and initialize
                idx = idx + 1;
                dataSet(idx).name = strcat(name, sprintf('%d',dset));;
                dataSet(idx).nameDep =  strcat(nameDep, sprintf('%d',dset));
                dataSet(idx).dep = 1;
                dataSet(idx).data = zeros(1,numdatapoints);
                idata = 1;
            end

            % check end of section set independent variable and set length
            if strcmp(line(1:min(8,ll)),'</indep>')
                dataSet(idx).data = dataSet(idx).data(1:idata-1);
                dataSet(idx).len = idata-1;
                idata = 0;
            end
            
            % check end of dependent variable section
            if strcmp(line(1:min(6,ll)),'</dep>')
                dataSet(idx).data = dataSet(idx).data(1:idata-1);
                dataSet(idx).len = idata-1;
                idata = 0;
                dset = 1;   
            end
        else
            if idata > 0
                % check if number is complex
                jloc = strfind(line, 'j');
                % datum is a real value
                if isempty(jloc)
                    [val,cnt] = sscanf(line,'%g');
                    if cnt ~= 1
                        error('QUCS:loaddataset:badnum','Invalid data in data set %s\n', dataSetFile);
                    end
                else % datum is complex
                    line(jloc) = line(jloc-1);
                    line(jloc-1) = ' ';
                    [val,cnt] = sscanf(line,'%g');
                    if cnt ~= 2
                        error('QUCS:loaddataset:badnum', 'Invalid data in data set %s\n', dataSetFile);
                    end
                    val = complex(val(1), val(2));
                end
                
                % idata exceeds indep numdatapoints then increment data set
                if idata > numdatapoints  
                    % preserve last data and set length
                    dataSet(idx).data = dataSet(idx).data(1:idata-1);
                    dataSet(idx).len = idata-1;
                  
                    % increment data set and index
                    if dset < nset
                        dset = dset + 1;
                        idx = idx + 1;
                        idata = 1;
                  
                        % update name with next data set
                        dataSet(idx).name = strcat(name, sprintf('%d',dset));
                        dataSet(idx).nameDep = strcat('Number', sprintf('%d',dset));
                        dataSet(idx).dep = 1;
                        dataSet(idx).data = zeros(1,numdatapoints);
                    end
                end      
                % add datum to array and increment
                dataSet(idx).data(idata) = val;
                idata = idata + 1;
            end        
        end
    end
    fclose(fid);

end
