<?php

$fp = fopen("/home/kadu0922/log/data.json", "r");
while ($line = fgets($fp)) {
  echo "$line<br />";
}
fclose($fp);

?>