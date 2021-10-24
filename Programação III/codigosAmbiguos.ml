(* Inputs *)
let input1 () = [
	('a', [0;1;0]);
    ('c', [0;1]);
    ('j', [0;0;1]);
    ('l', [1;0]);
    ('p', [0]);
    ('s', [1]);
    ('v', [1;0;1])
    ]


let input2 () = [
	('a', [0;1;1;0]);
	('b', [0;1;1;1;1;1]);
	('c', [1;1;0;0;1;1;1;1]);
	('f', [1;0;1;1;1;0]);
	('j', [0;1;0]);
	('l', [0;1;0;0]);
	('r', [0;1;1;1;0])
	]


(* Input que será testado *)
let input () = 
	input2 ()


(* Imprime Inputs pelo método print_list *)
(* ------------------------------------- *)
let rec print_array = function
	| [] -> ()
	| [x] -> print_int x
	| x::a -> print_int x; print_string ";"; print_array a

let print_arguments = function
	(x, a) -> print_string x; print_string ", "; print_array a

let rec print_list = function
	| [] -> ()
	| x::l -> print_arguments x; print_string "\n"; print_list l
(* ------------------------------------- *)
(* ------------------------------------- *)


(* Imprime uma lista de chars *)
let rec print_chars = function
	| [first] -> print_char first
	| first::last -> print_char first; print_string ";"; print_chars last
	| _ -> failwith "not possible"


(* Imprime o Output *)
let print_finalResult = function
	| (first,middle,last) -> print_string "([";
							 print_array first;
							 print_string "], [";
							 ignore (print_chars middle);
							 print_string "], [";
							 ignore (print_chars last);
							 print_string "])\n"


(* Converte uma string para uma lista de chars *)
let convertStringToList str =
	let rec convertStringToListAux i l =
		if i < 0 then l
		else convertStringToListAux (i - 1) (str.[i] :: l)
	in
		convertStringToListAux (String.length str - 1) []


(* Compara duas listas e retorna verdadeiro se forem iguais *)
let rec compareLists list1 list2 = match (list1, list2) with
	| [], [] -> true
	| [],_ -> false
	| _,[] -> false
	| (first1::last1), (first2::last2) -> if first1 != first2 then false else compareLists last1 last2


(* Retorna o primeiro elemento dos argumentos *)
let getLastPositionArgument = function
    | (first, last) -> last


(* Retorna o último elemento dos argumentos *)
let getFirstPositionArgument = function
    | (first, last) -> first


(* Retorna o primeiro argumento da lista *)
let getFirstPositionLists = function
	| [first] -> first
    | first :: last -> first
    | _ -> failwith "not possible"
 

(* Verifica se o array y está contido deste o início ao array x *)
let arrayInArray x y =
	if Array.length x >= Array.length y then Array.sub x 0 (Array.length y) = y else false;;


(* Remove as primeiras n posições do array *)
let removeArrayFromN x n =
	let rec removeArrayFromNAux x n =
		if n > 0 then removeArrayFromNAux (Array.sub x 1 (Array.length x -1)) (n-1)
		else x
	in removeArrayFromNAux x n


(* Converte os chars do input inicial para string *)
let convertCodesToString code = 
	let rec convertCodesToStringAux newcode = function
		| [] -> newcode
		| first::last -> convertCodesToStringAux 
							(List.append newcode [(String.make 1 (getFirstPositionArgument first), getLastPositionArgument first)])
							last
	in
		convertCodesToStringAux [] code;;


(* Multiplica o input original com o atual *)
let multiplyCode code generatedCode =
	let rec multiplyCodeAux newcode = function
		| [] -> (List.append newcode code)
		| first::last -> let rec multiplyArgument newcodeAux = function
							| [] -> multiplyCodeAux newcodeAux last
							| firstAux::lastAux -> multiplyArgument
							 							(List.append 
							 								newcodeAux 
							 								[(String.concat
								 								(getFirstPositionArgument first)
								 								["";(getFirstPositionArgument firstAux)],
								 							List.append
								 								(getLastPositionArgument first)
								 								(getLastPositionArgument firstAux))])
							 							lastAux
						 in
						 	multiplyArgument newcode code
	in
		multiplyCodeAux [] generatedCode


(* Encontra a menor sequencia dado a string, a sua sequencia e a lista a ser testada*)
let findSequence str sequence codes = 
	let rec findSequenceAux save saveAux auxSequence = function
		| [] -> save
		| first::last -> if arrayInArray auxSequence (Array.of_list (getLastPositionArgument first)) then
							(* Verifica se a sequencia acabou *)
							if removeArrayFromN auxSequence (Array.length (Array.of_list (getLastPositionArgument first))) = [||] then 
								(* Verifica se o saveAux é menor que o save *)
								if not (compareLists
											(convertStringToList str)
											(List.append (saveAux) (convertStringToList (getFirstPositionArgument first))))
								&& (save = [] || List.length save > (List.length saveAux +1)) then
									findSequenceAux
										(List.append (saveAux) (convertStringToList (getFirstPositionArgument first)))
										saveAux
										auxSequence
										last
								else
									findSequenceAux save saveAux auxSequence last
							(* Se a sequencia não acabou irá continuar pra frente*)
						 	else
						 		findSequenceAux
						 			(findSequenceAux 
							 			save 
							 			(List.append (saveAux) (convertStringToList (getFirstPositionArgument first))) 
							 			(removeArrayFromN auxSequence (Array.length (Array.of_list (getLastPositionArgument first))))
							 			 codes)
							 		saveAux 
							 		auxSequence 
							 		last
						 (* Se a sequencia não estiver contida não irá continuar pra frente*)
						 else 
						 	findSequenceAux save saveAux auxSequence last	 	
	in
		findSequenceAux [] [] sequence codes;;


(* Método que dado um código ambíguo retorna a mensagem codificada mais curta (caso haja várias, a primeira por ordem lexicográfica) *)
let ambiguo codes =
	let rec ambiguous_codesAux save saveLetra saveSequencia n newcodes = function
		| [] -> if save != [] then (saveSequencia, convertStringToList saveLetra, save)
				else 
					if (List.length codes) > String.length (getFirstPositionArgument (getFirstPositionLists newcodes)) then
						ambiguous_codesAux [] " " [0] 0 (multiplyCode codes newcodes) (multiplyCode codes newcodes)
					else (saveSequencia, convertStringToList saveLetra, save)
		| first::last -> if List.length (findSequence
												(getFirstPositionArgument first)
												(Array.of_list (getLastPositionArgument first))
												newcodes) != 0
							&& (save = [] 
								||
								List.length save > List.length (findSequence
																	(getFirstPositionArgument first)
																	(Array.of_list (getLastPositionArgument first))
																	newcodes))
						 then
							ambiguous_codesAux 
								(findSequence 
									(getFirstPositionArgument first)
									(Array.of_list (getLastPositionArgument first))
									newcodes)
								(getFirstPositionArgument first)
								(getLastPositionArgument first)
								(n+1)
								newcodes
								last
						 else
						 	if List.length save
						 	   =
						 	   List.length (findSequence 
		 										(getFirstPositionArgument first)
		 										(Array.of_list (getLastPositionArgument first))
		 										newcodes) then
						 		if  List.length saveSequencia > List.length (getLastPositionArgument first) then
						 			ambiguous_codesAux 
										(findSequence 
											(getFirstPositionArgument first)
											(Array.of_list (getLastPositionArgument first))
											newcodes)
										(getFirstPositionArgument first)
										(getLastPositionArgument first)
										(n+1)
										newcodes
										last
						 		else
						 			ambiguous_codesAux 
								 		save 
								 		saveLetra 
								 		saveSequencia 
								 		(n+1)
								 		newcodes
								 		last
						 	else
							 	ambiguous_codesAux 
							 		save 
							 		saveLetra 
							 		saveSequencia 
							 		(n+1)
							 		newcodes
							 		last
	in 
		ambiguous_codesAux [] " " [0] 0 codes codes;;


(* Método main *)
let main () =
	print_finalResult (ambiguo (convertCodesToString (input ())));;


main ();;
