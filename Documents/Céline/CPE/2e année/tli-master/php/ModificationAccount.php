<?php
//page php gerant la modification d'informations sur l'utilisateur
session_start();
require('model.php');
// on recupere toutes les donnees modifiees ou non et on les reinitialise dans le base de donnee
	$nom= $_POST['nom'];
	$prenom= $_POST['prenom'];
	$pseudo= $_POST['pseudo'];
	$mail= $_POST['mail'];
	$id=$_SESSION['id'];
modificationInfoCustumers($nom,$prenom,$mail,$pseudo,$id);// fonction modifiant les donnes a partir de l'id
$_SESSION['pseudo'] = $pseudo;// on recharge le pseudo car modification possible
require('IndexMyAccount.php');// on revient sur IndexMyAccount

?>
