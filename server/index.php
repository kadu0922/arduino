<?php
if(isset($_POST)){
    foreach($_POST as $Key => $Value) $jsonstr =  json_encode($_POST);
    $fp = fopen("/home/kadu0922/log/data.json", "a");
    fwrite($fp,$jsonstr);
    fclose($fp);
}
?>