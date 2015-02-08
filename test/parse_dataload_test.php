<?php

require_once(dirname(__FILE__) . '/../parse_dataload.php');

class ParseTest extends PHPUnit_Framework_Testcase {
  public function testParseDataload() {
    $expected = json_decode(
        file_get_contents(dirname(__FILE__) . '/dataload.json'), true);
    $actual = ParseDataload::Parse(
        file_get_contents(dirname(__FILE__) . '/dataload.html'));
    $this->assertEquals($expected, $actual);
  }
}
