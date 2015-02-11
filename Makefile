help:
	@echo 'Usage: make (help|test)'
.PHONY: help

run:
	php main.php

test:
	php test/phpunit.phar test/*_test.php
.PHONY: test

average_map: bin/average_map
.PHONY: average_map

bin/%: %.cc
	mkdir -p bin
	c++ -std=c++0x -O2 -o bin/$* $*.cc
