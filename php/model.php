<?php
//model.php est une librairie effectuant des requetes mysql 
function inscription($password,$pseudo,$mail,$nom,$prenom)// fonction permettant d'inserer une ligne dans la base de donnee
{
$bdd=connexionDB();
$req = $bdd->prepare('INSERT INTO InfoClient(Nom, Prenom,mail, pseudo, password) VALUES(:Nom, :Prenom ,:mail,:pseudo,  :password)');
$req->execute(array(

     	'Nom' => $nom,
	'Prenom' => $prenom,
	'mail' => $mail,
    	'pseudo' => $pseudo,    
   	'password' => $password,

    ));
}

function connexionDB(){//Fonction permettant a l'utilisateur de se connecter a la base de donner TODO: rendre cela generale car ici specifique
try{
$bdd = new PDO('mysql:host=localhost;dbname=TLI;charset=utf8', 'root', '');
}
catch(Exception $e)
{
        die('Erreur : '.$e->getMessage());
}
return $bdd;


}

function connexionWebSite($pseudo,$mdp){
$resultat=informationCustumers($pseudo);


// Comparaison du pass envoyé via le formulaire avec la base



$isPasswordCorrect = password_verify($mdp, $resultat['password']);
$resultsFinal = array ($resultat,$isPasswordCorrect);

return $resultsFinal;

}

function informationCustumers($pseudo){// renvoie les informations sur un client 
$bdd = connexionDB();
$req = $bdd->prepare('SELECT id, Nom, Prenom, mail, password FROM InfoClient WHERE pseudo = :pseudo');

$req->execute(array(

    'pseudo' => $pseudo));

$resultat = $req->fetch();
return $resultat;



}

function modificationInfoCustumers($nom,$prenom,$mail,$pseudo,$id){// permet de modifier les informations sur un client a partir de son id (sauf mot de passe)
$bdd=connexionDB();
$req = $bdd->prepare('UPDATE InfoClient SET Nom = :Nom, Prenom =:Prenom ,mail = :mail,pseudo = :pseudo WHERE id = :id');
$req->execute(array(

     	'Nom' => $nom,
	'Prenom' => $prenom,
	'mail' => $mail,
    	'pseudo' => $pseudo,    
   	//'password' => $password,
	'id'=>$id,

    ));


}



function getNews()

{

$bdd=connexionDB();


$req = $bdd->query('SELECT id, titre, contenu, DATE_FORMAT(date, \'%d/%m/%Y à %Hh%imin%ss\') AS date_creation_fr FROM news ORDER BY date DESC LIMIT 0, 5');


    return $req;

}

function PseudoExist($pseudo){

$bdd=connexionDB();
$req = $bdd->prepare('SELECT id FROM InfoClient WHERE pseudo = :pseudo');
$req->execute(array(

    'pseudo' => $pseudo));

if($req->fetch()){
	return TRUE;
}
else{
	return FALSE;
}
}

function MailExist($mail){
$bdd=connexionDB();
$req = $bdd->prepare('SELECT id FROM InfoClient WHERE mail = :mail');
$req->execute(array(

    'mail' => $mail));

if($req->fetch()){
	return TRUE;
}
else{
	return FALSE;
}


}

function GetPassword($pseudo){
$bdd = connexionDB();
$req = $bdd->prepare('SELECT password FROM InfoClient WHERE pseudo = :pseudo');

$req->execute(array(

    'pseudo' => $pseudo));

$resultat = $req->fetch();
return $resultat;
}


function PasswordModification($pseudo,$oldPassword,$newPassword){
$bdd = connexionDB();
$req=GetPassword($pseudo);
$realPw=$req['password'];
$mdpChange=FALSE;
if (password_verify($oldPassword, $realPw)){
	$req = $bdd->prepare('UPDATE InfoClient SET password = :password WHERE pseudo = :pseudo');
	$req->execute(array(

    	'pseudo' => $pseudo,    
   	'password' => $newPassword,

    ));
	return !$mdpChange;
}
else{
return $mdpChange;
}


}

?>

