package trabalho;

import java.util.*;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

public interface ProductRepository extends JpaRepository<Product, Long> {
	@Query("SELECT n FROM Product n WHERE n.name = ?1")
	public Product findByName(String name);

	@Query("SELECT i FROM Product i WHERE i.id = ?1")
	public Product findByID(Long id);

	List<Product> findByNameContaining(String name);
}
