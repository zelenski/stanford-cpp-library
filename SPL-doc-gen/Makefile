all: custom

QT_BIN = /Applications/Qt/5.14.2/clang_64/bin/

q:
	rm -fr qt_docs/html
	$(QT_BIN)/qdoc --prepare qt_customizations/all.qdocconf
	$(QT_BIN)/qdoc --generate --highlighting -write-qa-pages  qt_customizations/all.qdocconf
	$(QT_BIN)/qhelpgenerator qt_docs/html/all.qhp -o qt_docs/qtdocpackage.qch

custom:
	rm -rf doxy_out/html_before doxy_out/html_after
	DOX_INTERNAL=YES DOX_SECTIONS=INTERNAL doxygen Doxyfile
	@doxyCleanup.py doxy_out/html_before/ doxy_out/html_after/
	$(QT_BIN)/qhelpgenerator -s doxy_out/html_after/*.qhp -o doxy_out/html_after/cleaned.qch

stock:
	rm -rf doxy_out/html_stock
	doxygen StockDoxyfile

clean:
	rm -rf doxy_out