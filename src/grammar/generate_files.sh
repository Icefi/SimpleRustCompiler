#!/bin/bash

readonly antlr=../../../external/antlr-4.9.1-complete.jar

cd grammar
java -jar $antlr -Dlanguage=Cpp RustLang.g4 -visitor
