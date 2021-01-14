<?php
$jsonPath = "/home/www-data/log/data.json";
if(isset($_POST)){
    $jsonstr = json_decode(file_get_contents($jsonPath),true);
    $fp = fopen($jsonPath, "w");
    $_POST["T"] = -45+175*hexdec($_POST["T"])/65535;
    $_POST["H"] = 100*hexdec($_POSaT["H"])/65535;
    $_POST["V"] = 3.0/4095*hexdec($_POST["V"]);
    $jsonstr[]=$_POST;
    fwrite($fp,json_encode($jsonstr));
    fclose($fp);
}
?>