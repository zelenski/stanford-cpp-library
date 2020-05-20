
This subproject is where I am trying to build system that can regenerate up-to-date docs from SPL header files.

__Timeline__
- Aug 2019  Opened project
    doc format used in most current does not match any existing tool (cppdoc code rot), 
    some of graphics header use Doxygen-style
    last known re-gen was 2014, html files have only been sporadically hand-edited since
- Sept 2019 Explored options for doc generation
    Marty advice: "avoid Doxygen"
    I tried Qdoc but tools are mid-transition from old parser to clang-based, lots of stuff does not work, do not want to further tie ourselves further anything Qt-specific (in hopes we might someday CLion or other)
    Also looked at moxygen, appledoc (not quite right either)
- Dec 2019 Reluctantly back to Doxygen
    Parsing engine seems to be best out there
    Reliably generates HTML but ugly, overkill, inflexible
    Tried using xml output, but that stops way short
    Start with HTML and post-process using BeautifulSoup 
    Not especially pleasing but have something that kind of works, needs tweaking
- Jan 2020
    Geez, SPL header files need work
    most markup is cppdoc (which nothing understands), some is javadoc, other is doxygen
    inconsistent voicing/wording


__Integration with QtCreator__
- code completion/tool tips are coming off live parse of header files
- use qlhelpgenerator to package HTML docs, will display in QT help view (can be packed into wizard or use online copy)
- Can also add standard c++ docs that way
