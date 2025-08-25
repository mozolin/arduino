<?

/*
=> from Arduino IDE: ds18b20_multi.ino
Found device 0 with address: 2861640AB0099763
Found device 1 with address: 2861640AB3499AE1
Found device 2 with address: 2861640AB379B712
*/


$templ = file_get_contents('_ds18b20_multi.template.yaml');

$arrStrings = [
	str_split('2861640AB0099763', 2),
	str_split('2861640AB3499AE1', 2),
	str_split('2861640AB379B712', 2),
];

$s = [
	'[ADDR]',
  '[NUM]',
  '[BIG_NUM]',
];

$idx = 1;
foreach($arrStrings as $arrItems) {
	$strTmp = '0x';
	//print_r($arrItems);
	//continue;
	for($i = 7; $i >= 0; $i--) {
		$strTmp .= $arrItems[$i];
		//$arrItems[$i]
	}
	$r = [
	   $strTmp,
	   $idx,
	   str_pad($idx, 2, "0", STR_PAD_LEFT),
	];
	$strTempl = str_replace($s, $r, $templ);
	
	echo $strTempl."\n";
	$idx++;
}
