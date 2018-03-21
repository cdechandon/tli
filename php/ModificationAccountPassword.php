<?php
//page php gerant la modification d'informations sur l'utilisateur
session_start();
require('model.php');
// on recupere toutes les donnees modifiees ou non et on les reinitialise dans le base de donnee

	$oldPassword=$_POST['pwd1'];
	$newPassword=$_POST['pwd2'];

if(isset($oldPassword)&isset($newPassword)){
if(strlen($newPassword)>=7){
$repMdp=PasswordModification($_SESSION['pseudo'],$oldPassword,password_hash($newPassword, PASSWORD_DEFAULT));
}
else{
	$repMdp=FALSE;
}
}



require('IndexMyAccount.php');// on revient sur IndexMyAccount

?>
