<?php
// page php gerant en MVC la page Utilisateur
require_once('model.php');// On recupere les fonctions SQL
require("smartyLibs/Smarty.class.php");
$tpl = new Smarty();



$resultat=informationCustumers($_SESSION['pseudo']);// On recupere les infos sur le client
$tpl->assign('Name', $resultat['Nom']);

$tpl->assign('FName', $resultat['Prenom']);
$tpl->assign('Mail', $resultat['mail']);

$tpl->assign('Pseudo',$_SESSION['pseudo']);





// Ici par default les erreurs sont vides car lors de la premiere connection il n'y a aucune erreur
$tpl->assign('InfoPseudo', "");
$tpl->assign('InfoMail', "");
$tpl->assign('InfoPassword', "");
if(isset($repMdp)){
	if(!$repMdp){
		$tpl->assign('InfoPassword', "Veuillez recommencer, mauvais mot de passe");}
	
	else{
		$tpl->assign('InfoPassword', "Mot de passe modifie");}

}
if(isset($errorMail)){
	$tpl->assign('InfoMail', $errorMail);}
if(isset($errorPseudo)){
	$tpl->assign('InfoPseudo', $errorPseudo);}



$tpl->display('../html/MyAccount.html');
?>
