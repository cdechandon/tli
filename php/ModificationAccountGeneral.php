<?php
//page php gerant la modification d'informations sur l'utilisateur
session_start();
require_once('model.php');
require('inscriptionCheck.php');
// on recupere toutes les donnees modifiees ou non et on les reinitialise dans le base de donnee
	

	$nom= $_POST['lastName'];
	$prenom= $_POST['firstName'];
	$pseudo= $_POST['login'];
	$mail= $_POST['mail'];
	$id=$_SESSION['id'];
	$login=$_SESSION['pseudo'];
	$mailb=$_SESSION['mail'];

if(isset($nom)&isset($prenom)&isset($pseudo)&isset($mail)&isset($id)){// on teste l'existance des variables

$compte = new Compte($pseudo,"",$mail,$nom,$prenom); // On creer une instance de compte
if(!($compte->verifNom())){
	$errorName="Le nom que vous avez rentre n'est pas valide";// message d'erreur a afficher si nous rentrons dans une des boucles
}
if(!($compte->verifPrenom())){
	$errorFName="Le prenom que vous avez rentre n'est pas valide";
}
if($login !=$pseudo){
	if(!($compte->verifModifPseudo())){
		$errorPseudo="Le pseudo que vous avez rentre n'est pas valide";
	}
}
if($mailb !=$mail){
	if(!($compte->verifMail())){
		$errorMail="Le mail que vous avez rentre n'est pas valide";
	}
}
if(!(isset($errorName)|isset($errorFName)|isset($errorPseudo)|isset($errorMail))){

modificationInfoCustumers($nom,$prenom,$mail,$pseudo,$id);// fonction modifiant les donnes a partir de l'id
$_SESSION['pseudo'] = $pseudo;// on recharge le pseudo car modification possible
}
}



require('IndexMyAccount.php');// on revient sur IndexMyAccount

?>
