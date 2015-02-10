<?php

date_default_timezone_set('UTC');

$cache = [];
foreach (glob('cache/*') as $file) {
  $cache = array_merge($cache, json_decode(file_get_contents($file), true));
}
ksort($cache);

$prices = [];
foreach ($cache as $date => $price) {
  $date = round(strtotime($date) / 24 / 3600);
  echo "$date\t{$price['close']}\n";
}
