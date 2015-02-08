<?php

require_once(dirname(__FILE__) . '/library/simple_html_dom.php');

class ParseDataload {
  public static function Parse($data) {
    return self::ExtractPrices(str_get_html($data));
  }

  private static function ExtractPrices($html) {
    $stats = [];
    foreach ($html->find('table') as $table) {
      foreach ($table->find('tr') as $tr) {
        $daily_stat = [];
        foreach ($tr->find('td') as $td) {
          $stats = array_merge($stats, self::ExtractPrices($td));
          $daily_stat[] = $td->plaintext;
        }
        if (count($daily_stat) == 5 &&
            preg_match('%^\d{4}/\d\d/\d\d$%', $daily_stat[0])) {
          $date = str_replace('/', '-', array_shift($daily_stat));
          $daily_stats = [];
          foreach (array_values($daily_stat) as $value) {
            if (trim($value) == '-') {
              $daily_stats[] = NULL;
            } else {
              $daily_stats[] = floatval(str_replace(',', '', $value));
            }
          }
          $stats[$date] = [
            'open' => $daily_stats[0],
            'high' => $daily_stats[1],
            'low' => $daily_stats[2],
            'close' => $daily_stats[3]];
        }
      }
    }
    return $stats;
  }
}
