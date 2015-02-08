help:
	@echo 'Usage: make (help|test)'
.PHONY: help

run:
	php main.php

test:
	php test/phpunit.phar test/*_test.php
.PHONY: test
