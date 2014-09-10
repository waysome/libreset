add_subdirectory(fig)

add_custom_command(
    OUTPUT main.pdf
    COMMAND pdflatex main
    COMMAND pdflatex main
    MAIN_DEPENDENCY main.tex
    DEPENDS figures
)

add_custom_target(paper DEPENDS main.pdf)


