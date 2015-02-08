<?php

require_once(dirname(__FILE__) . '/parse_dataload.php');

date_default_timezone_set('UTC');

if (!is_dir('cache')) {
  mkdir('cache', 0777);
}

for ($month = '1970-01'; $month < date('Y-m');
     $month = date('Y-m', strtotime('next month',
                                    strtotime($month . '-01')))) {
  $output_path = "cache/$month.json";
  if (file_exists($output_path)) {
    fwrite(STDERR, "Skipping $month...\n");
    continue;
  } else {
    fwrite(STDERR, "Downloading $month...\n");
  }
  $time = strtotime($month . '-01');
  $url = 'http://indexes.nikkei.co.jp/nkave/statistics/dataload?' .
         'type=daily&year=' . date('Y', $time) . '&month=' . date('n', $time);
  file_put_contents(
      $output_path,
      json_encode(ParseDataload::Parse(file_get_contents($url)),
                  JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE) . "\n");
  sleep(1);
}
