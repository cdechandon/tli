<?php 


$pseudo=$_POST['pseudo'];

$bdd = new PDO('mysql:host=localhost;dbname=TLI;charset=utf8', 'root', '');


$req = $bdd->prepare('SELECT id, password FROM InfoClient WHERE pseudo = :pseudo');

$req->execute(array(

    'pseudo' => $pseudo));

$resultat = $req->fetch();


// Comparaison du pass envoyé via le formulaire avec la base
echo $resultat['password'];


$isPasswordCorrect = password_verify($_POST['mdp'], $resultat['password']);// password_verify hache le mot de passe et le compare a celui recuperer dans la base de donnee


if (!$resultat)

{

    echo 'Mauvais identifiant ou mot de passe !';

}

else

{

    if ($isPasswordCorrect) {

        session_start();

        $_SESSION['id'] = $resultat['id'];

        $_SESSION['pseudo'] = $pseudo;

        echo 'Vous êtes connecté !';

    }

    else {

        echo 'Mauvais identifiant ou mot de passe !';

    }

}
