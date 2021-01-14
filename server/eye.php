<?php

$fp = fopen("/home/www-data/log/data.json", "r");
while ($line = fgets($fp)) {
  echo "$line<br />";
}
fclose($fp);

?>