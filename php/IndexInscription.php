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

if(isset($errorPseudo)){
	$tpl->assign('InfoPseudo', $errorPseudo);}


$tpl->display('../html/AccueilInscription.html');

?>
