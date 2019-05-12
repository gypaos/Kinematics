include ./Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      = libRal.so libTarget.so \
                libKinematics.so    libDisplayBrho.so \
                libEnsdf.so         libSpectroPlot.so \
                libEnsdfPlot.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------

libEnsdf.so:	TEnsdf.o   TEnsdfDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TEnsdfDict.cxx:	TEnsdf.h
		rootcint -f $@ -c $^

libEnsdfPlot.so:	TEnsdfPlot.o   TEnsdfPlotDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TEnsdfPlotDict.cxx:	TEnsdfPlot.h
		rootcint -f $@ -c $^

libNoyau.so:	TNoyau.o   TNoyauDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TNoyauDict.cxx:	TNoyau.h
		rootcint -f $@ -c $^

libBeam.so:	TBeam.o		   TBeamDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TBeamDict.cxx:	TBeam.h
		rootcint -f $@ -c $^

libRal.so:	TRal.o TRalDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TRalDict.cxx:	TRal.h
		rootcint -f $@ -c $^

libTarget.so:	TTarget.o TTargetDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTargetDict.cxx:	TTarget.h
		rootcint -f $@ -c $^

libReaction.so:	TReaction.o	   TReactionDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TReactionDict.cxx:    TReaction.h
		      rootcint -f $@ -c $^

libEmission.so:		TEmission.o	TEmissionDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TEmissionDict.cxx:	TEmission.h
			rootcint -f $@ -c $^

libDisplayBrho.so:		TDisplayBrho.o	TDisplayBrhoDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TDisplayBrhoDict.cxx:	TDisplayBrho.h
			rootcint -f $@ -c $^

libSpectroPlot.so:		TSpectroPlot.o	TSpectroPlotDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSpectroPlotDict.cxx:	TSpectroPlot.h
			rootcint -f $@ -c $^

libKinematics.so:	TReaction.o	TReactionDict.o   \
                  TNoyau.o    TNoyauDict.o      \
                  TBeam.o     TBeamDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

clean:
		@rm -f core *~ *.o *Dict*

distclean:
	  make clean; rm -f *.so

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $< -L./ -I/usr/include

# dependances
TEnsdf.o:         TEnsdf.cxx        TEnsdf.h
TEnsdfPlot.o:     TEnsdfPlot.cxx    TEnsdfPlot.h
TNoyau.o:         TNoyau.cxx	      TNoyau.h
TBeam.o:          TBeam.cxx	      TBeam.h
TReaction.o:      TReaction.cxx     TReaction.h
TDisplayBrho.o:   TDisplayBrho.cxx	TDisplayBrho.h
TSpectroPlot.o:   TSpectroPlot.cxx	TSpectroPlot.h
TRal.o:	         TRal.cxx	         TRal.h
TTarget.o:	      TTarget.cxx	      TTarget.h
