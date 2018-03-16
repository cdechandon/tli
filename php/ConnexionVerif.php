<?php 
//Page php permettant de verifier l'existance du compte et la validite du MDP
//require('model.php');
require('inscriptionCheck.php');

$pseudo=$_POST['pseudo'];// recupere les variables de la methode POST
$mdp=$_POST['mdp'];
$resultsFinal=connexionWebsite($pseudo,$mdp);


if (!$resultsFinal[0]) // $resultsFinal[0] contient la ligne du tableau contenant les infos sur l'utilisateur, si il n'y a rien on rentre dans le if

{

    echo 'Mauvais identifiant ou mot de passe !';

}

else

{

    if ($resultsFinal[1]) {// $resultsFinal[1] contient true si le mdp indiquee est le meme que celui stocké

        session_start();// ouverture d'une session

        $_SESSION['id'] = $resultsFinal[0]['id'];// celle-ci contient l'id dans le base de donnee(unique)

        $_SESSION['pseudo'] = $pseudo;// On lui donne aussi le pseudo de l'utilisateur
	require('IndexAccueil.php');

    }

    else {

        echo 'Mauvais identifiant ou mot de passe !';

    }

}
