<?php
// page php gerant l'accueil en MVC
session_start();// ouverture session
if (isset($_SESSION['id']) AND isset($_SESSION['pseudo']))// on indique a l'utilisateur s'il est connectee ou non

{

    echo 'Bonjour ' . $_SESSION['pseudo'];

}
else {
echo 'pas connecter';
}


require('model.php');
$req = getNews();
require('../html/pageAcceuil.html');// on affiche la page html d'acceuil

?>
