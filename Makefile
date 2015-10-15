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

growth: bin/growth
.PHONY: growth

etf: bin/etf
.PHONY: etf

etf_growth: bin/etf_growth
.PHONY: etf_growth

etf_dynamic_growth: bin/etf_dynamic_growth
.PHONY: etf_dynamic_growth

bin/%: %.cc
	mkdir -p bin
	c++ -std=c++0x -O2 -o bin/$* $*.cc
