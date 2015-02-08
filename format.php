<?php

$cache = [];
foreach (glob('cache/*') as $file) {
  $cache = array_merge($cache, json_decode(file_get_contents($file), true));
}
ksort($cache);

$prices = [];
foreach ($cache as $date => $price) {
  $prices[] = $price;
}

for ($i = 0; $i < count($prices); $i += 1) {
  echo $prices[$i]['close'] . "\n";
}
