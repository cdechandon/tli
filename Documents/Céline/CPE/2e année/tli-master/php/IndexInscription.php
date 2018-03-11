<?php
//Page php gerant en MVC l'inscription
require("smartyLibs/Smarty.class.php");
$tpl = new Smarty();
$tpl->assign('InfoName', "");
$tpl->assign('InfoFName', "");
$tpl->assign('InfoPseudo', "");
$tpl->assign('InfoMail', "");
$tpl->assign('InfoPassword', "");


if(isset($errorMail)){
	$tpl->assign('InfoMail', $errorMail);}
if(isset($errorName)){
	$tpl->assign('InfoName', $errorName);}
if(isset($errorFName)){
	$tpl->assign('InfoFName', $errorFName);}
if(isset($errorPseudo)){
	$tpl->assign('InfoPseudo', $errorPseudo);}
if(isset($errorPassword)){
	$tpl->assign('InfoPassword', $errorPassword);}

$tpl->display('../html/AccueilInscription.html');

?>
