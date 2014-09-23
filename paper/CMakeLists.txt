add_subdirectory(fig)


#
# All the TeX files
#
set(TEX_FILES
    AVL-ops.tex
    bloom-filter.tex
    data-layout.tex
    intro.tex
    main.tex
    misc.tex
    operations.tex
    overview.tex
    parameters.tex
)


#
# Building facilities for the paper
#
add_custom_command(
    OUTPUT main.pdf
    COMMAND pdflatex -halt-on-error main
    COMMAND pdflatex -halt-on-error main
    MAIN_DEPENDENCY main.tex
    DEPENDS figures ${TEX_FILES}
)

add_custom_target(paper DEPENDS main.pdf)


