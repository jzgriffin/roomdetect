LUALATEX ?= lualatex

.PHONY: all clean distclean
all: report.pdf
clean:
	$(RM) -r report.log report.aux
distclean: clean
	$(RM) report.pdf

%.pdf: %.tex
	$(LUALATEX) $(LUALATEXFLAGS) $<
