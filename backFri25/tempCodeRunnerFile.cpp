  for (int i = 0 ; i < 17 ; i++){
        state[i+6]=0;
    }

    std::cout <<  "\n New values after the states tailed with zeros\n ";
        // Print the updated state array
    for (int i = 0; i < 25; ++i) {
        std::cout << state[i] << " ";
    }