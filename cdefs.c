
// to implement hashing 
int hashCode(int key) {
   return key % SIZE;
}


// modify later.
int hash_insert(struct my_struct *data, struct hash_table *hash_table) {
    int try, hash;
    if(hash_table->number_of_elements >= hash_table->max) {
        return 0; // FULL
    }
    for(try = 0; true; try++) {
        hash = hash_fun(data->key, try, hash_table->max);
        if(hash_table->elements[hash] == 0) { // empty cell
            hash_table->elements[hash] = data;
            hash_table->number_of_elements++;
            return 1;
        }
    }
    return 0;
}

struct my_struct *hash_retrieve(int key, struct hash_table *hash_table) {
    int try, hash;
    for(try = 0; true; try++) {
        hash = hash_fun(key, try, hash_table->max);
        if(hash_table->elements[hash] == 0) {
            return 0; // Nothing found
        }
        if(hash_table->elements[hash]->key == key) {
            return hash_table->elements[hash];
        }
    }
    return 0;
}