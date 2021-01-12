<?php
$jsonPath = "/home/www-data/log/data.json";
if(isset($_POST)){
    $jsonstr = json_decode(file_get_contents($jsonPath),true);
    $fp = fopen($jsonPath, "w");
    $jsonstr[]=$_POST;
    fwrite($fp,json_encode($jsonstr));
    fclose($fp);
}
?>