<?php 
if(!$h=fopen("lora.help","a")){
	echo "Not USE";
}

if (empty($_GET["lora"])&& empty($_GET["rssi"])) {
	fwrite($h,"LORA esta vacio");
}else{
	$msg=$_GET["lora"]. " - RSSI: " .$_GET["rssi"]." - ". date("d-m-Y") . " - " . date("h:i:s a") . "\n";
}
fclose($h);
if(!$f=fopen("lora.txt","a")){
	echo "Not OK";
}
fwrite($f,$msg);
fclose($f);
?>
