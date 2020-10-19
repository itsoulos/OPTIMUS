<?php
	header('Content-Type: text/html; charset=utf-8');
	include_once("mysql.php");
	$link=mysqli_connect($dbhost,$dbuser,$dbpass);
	if(!$link) die("0");
	mysqli_select_db($link,$database);
	//create table here if not exists
	$sql="create table if not exists genclass(".
		"id INT(6) AUTO_INCREMENT PRIMARY KEY,".
		"name varchar(100),".
		"trainerror double,testerror double,bestprogram text)";
	if(!mysqli_query($link,$sql);)
		die("error $sql <br>");
	$name=$_POST["name"];
	$testerror=$_POST["testerror"];
	$trainerror=$_POST["trainerror"];
	$bestprogram=str_replace("#","+",$_POST["bestprogram"]);
	$sql="insert into genclass(name,trainerror,testerror,bestprogram) values('$name',$trainerror,$testerror,'$bestprogram')";
	$res=mysqli_query($link,$sql);
	if(!$res) 
	{
		mysqli_close($link);
		die("0");
	}
	mysqli_close($link);
	echo "1";
?>
