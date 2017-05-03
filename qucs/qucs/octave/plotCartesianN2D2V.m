% Octave plotting function plotCartesianN2D2V.
% Two data variable form.
%
% Function format
% plotCarteaianN2D2V(N, Type,
%		 XName, Xlabel, Xstart, Xstop,
%		 Y1Name, Y1label, Y1Colour, 
%		 Y2Name, Y2label, Y2Colour, Linewidth)
% Parameters
% N           Number of steps
% Type				Plot style "semilogx", "semilogy", "loglog", else "plot" (string)
% XName				X variable name (string).
% Xlabel                        X axis label (string).
% Xstart			X axis start value (real).
% Xstop				X axis stop value (real).
% Y1Name			Y1 variable name (string).
% Y1label			Y1 axis label (string).
% Y1Colour		Y1 plot colour (string).
% Y2Name			Y2 variable name (string).
% Y2label			Y2 axis label (string).
% Y2Colour			Y2 plot colour (string).
% Linewidth			Thickness of plotted line in pixels (integer).
%
% Script written with Octave 3.2.4
% Copyright 2012 by Mike Brinson
% Published under GNU General Public License (GPL V2). 
% No warranty at all.
%
% Modified 3/21/2017 by Michael Arendall miche.arendall@gmail.com
% Published under GNU General Public License (GPL v3). 
% No warranty at all.
% Modified to add plotting of dataSets containing N data vectors from indep Number number steps
%
function plotCartesianN2D2V(N, Type, 
                           XName,  Xlabel,  Xstart, Xstop,
                           Y1Name, Y1label,  Y1Colour,
                           Y2Name, Y2label,  Y2Colour,
                           Linewidth)
  if(nargin < 12)
     error("Too few input parameters.")
     return
  endif
  clf;
  if strcmp(Type, "plot") 
       subplot(2,1,1)      
       plot(XName, Y1Name, "linewidth", Linewidth, "color", Y1Colour);
       axis ([Xstart,Xstop]);
       ylabel(Y1label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       subplot(2,1,2)
       plot(XName, Y2Name, "linewidth", Linewidth, "color", Y2Colour);
       axis ([Xstart,Xstop] );
       ylabel(Y2label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       xlabel(Xlabel, "fontsize", 14, "fontname", "Arial", "fontweight", "bold" );
  elseif strcmp(Type, "semilogy") 
       subplot(2,1,1)
       semilogy(XName, Y1Name, "linewidth", Linewidth, "color", Y1Colour);
       axis ([Xstart,Xstop]);
       ylabel(Y1label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       subplot(2,1,2)
       semilogy(XName, Y2Name, "linewidth", Linewidth, "color", Y2Colour);
       axis ([Xstart,Xstop] );
       ylabel(Y2label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       xlabel(Xlabel, "fontsize", 14, "fontname", "Arial", "fontweight", "bold" );
  elseif strcmp(Type,"loglog")  
       subplot(2,1,1)
       loglog(XName, Y1Name, "linewidth", Linewidth, "color", Y1Colour);
       axis([Xstart, Xstop])
       ylabel(Y1label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       subplot(2,1,2)
       loglog(XName, Y2Name, "linewidth", Linewidth, "color", Y2Colour);
       axis([Xstart, Xstop])
       ylabel(Y2label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       xlabel(Xlabel, "fontsize", 14, "fontname", "Arial", "fontweight", "bold" );
  elseif strcmp(Type, "semilogx")
       subplot(2,1,1)
       semilogx(XName, Y1Name, "linewidth", Linewidth, "color", Y1Colour);
       axis ([Xstart,Xstop]);
       ylabel(Y1label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       subplot(2,1,2)
       semilogx(XName, Y2Name, "linewidth", Linewidth, "color", Y2Colour);
       axis ([Xstart,Xstop] );
       ylabel(Y2label, "fontsize", 14, "fontname", "Arial", "fontweight" ,"bold");
       xlabel(Xlabel, "fontsize", 14, "fontname", "Arial", "fontweight", "bold" );
  else
       % Do nothing!
  endif 
endfunction
  
