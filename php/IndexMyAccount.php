<?php
// page php gerant en MVC la page Utilisateur
require_once('model.php');// On recupere les fonctions SQL
$resultat=informationCustumers($_SESSION['pseudo']);// On recupere les infos sur le client
require('../html/MyAccount.html');// On affiche la page utilisateur

?>
