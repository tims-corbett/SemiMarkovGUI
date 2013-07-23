dot2tex.py -ftikz %1.DOT > %1.TEX
pdflatex -interaction=nonstopmode %1.TEX