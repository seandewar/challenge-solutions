package lasagna

func PreparationTime(layers []string, time int) int {
	if time == 0 {
		time = 2
	}
	return len(layers) * time
}

func Quantities(layers []string) (noodles int, sauce float64) {
	for i := 0; i < len(layers); i++ {
		if layers[i] == "noodles" {
			noodles += 50
		} else if layers[i] == "sauce" {
			sauce += 0.2
		}
	}
	return
}

func AddSecretIngredient(recipe, ingredients []string) {
	ingredients[len(ingredients)-1] = recipe[len(recipe)-1]
}

func ScaleRecipe(quantitiesFor2 []float64, portions int) []float64 {
	result := make([]float64, 0, len(quantitiesFor2))
	for i := 0; i < len(quantitiesFor2); i++ {
		result = append(result, (quantitiesFor2[i]/2)*float64(portions))
	}
	return result
}

// Your first steps could be to read through the tasks, and create
// these functions with their correct parameter lists and return types.
// The function body only needs to contain `panic("")`.
//
// This will make the tests compile, but they will fail.
// You can then implement the function logic one by one and see
// an increasing number of tests passing as you implement more
// functionality.
