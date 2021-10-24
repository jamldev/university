package trabalho;

import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

public interface OrderRepository extends JpaRepository<Order, Long> {
	@Query("SELECT i FROM Order i WHERE i.id = ?1")
	public Order findByID(Long id);

	@Query("SELECT i FROM Order i WHERE i.clientID = ?1")
	public List<Order> findByClientID(Long id);
}
