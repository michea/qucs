function qvect = parseQucsNDataSet(dataSet)
% Parse the contents of Qucs data set to the local data structure

% -*- texinfo -*-
% @deftypefn {Function File} parseQucsNDataSet (@var{dataset})
% Parses the content of the Qucs dataset @var{dataset} which contains Number of vectors.
% @end deftypefn

% Copyright 2017 (C) 2017 by Michael Arendall <miche.arendall@gmail.com`>
% Published under GNU General Public License (GPL v3). No warranty at all.
%
%
    % get the data names and create data vectors
    if isempty (dataSet)
        fprintf(1,'\nData set is empty.\n\n');
    else
        % create local vector struct and get the Qucs data
        for n = 1:length(dataSet)
            % qucs returns invalid '(' and ')' so convert to '_'
            qvect(n).name = genvarname(dataSet(n).name); 
            qvect(n).data = getQucsVariable(dataSet,dataSet(n).name);
            qvect(n).max = 1.0;
            qvect(n).min = -1.0;
            qvect(n).len = length(qvect(n).data);
            %fprintf(1, 'n = %d vector = %s len = %d\n', n, qvect(n).name, qvect(n).len);
        end
    end

endfunction
