<?php
// page php gerant l'accueil en MVC
session_start();// ouverture session
if (isset($_SESSION['id']) AND isset($_SESSION['pseudo']))// on indique a l'utilisateur s'il est connectee ou non

{

    echo 'Bonjour ' . $_SESSION['pseudo'];

}



require_once('model.php');
require("smartyLibs/Smarty.class.php");
$tpl = new Smarty();


$tpl->display('../html/pageAcceuil.html');// on affiche la page html d'acceuil


  


?>
