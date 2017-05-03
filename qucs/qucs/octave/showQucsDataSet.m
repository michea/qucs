function showQucsDataSet(dataSet)
% Display the contents of Qucs data set to the command line

% -*- texinfo -*-
% @deftypefn {Function File} showQucsDataSet (@var{dataset})
% Shows the content of the Qucs dataset @var{dataset}.
% @end deftypefn

% Copyright 2010 (C) 2010 by Eberhard Gamm <eberhard.gamm@gmx.de>
% Published under GNU General Public License (GPL V2). No warranty at all.
%
% Modified 3/21/2017 by Michael Arendall miche.arendall@gmail.com
% Published under GNU General Public License (GPL v3). No warranty at all.
%
% Modified to add optional printing of data values for each dataSet
    
	if isempty(dataSet)
        fprintf(1,'\nData set is empty.\n\n');
    else
        fprintf(1,'\nContents of data set:\n');
        fprintf(1,'------------------------------------------------------------\n');
        fprintf(1,'Variable                 Dependency                   Length\n');
        fprintf(1,'------------------------------------------------------------\n');
        for i = 1:length(dataSet)
            fprintf(1,'\n%-24s %-24s %10d\n',dataSet(i).name,...
                dataSet(i).nameDep,dataSet(i).len);

            % debug    
            %for j = 1:dataSet(i).len
            %    fprintf(1,'%f\n',dataSet(i).data(j));
            %end
            fprintf(1,'\n');    
        end
        fprintf(1,'------------------------------------------------------------\n\n');
    end

end
