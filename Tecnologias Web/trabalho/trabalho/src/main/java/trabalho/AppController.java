package trabalho;

import java.util.List;

import javax.servlet.http.*;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.PathVariable;

@Controller
public class AppController {

	public String ROLE="USER";

	@Autowired
	private UserRepository userRepo;

	@Autowired
	private OrderRepository orderRepo;

	@Autowired
	private ProductRepository productRepo;

	//Default:
	////////////////
	////////////////

	@GetMapping("/")
	public String defaultPage(HttpServletRequest request, Model model) {
		List<Product> listProducts = productRepo.findAll();
		model.addAttribute("listProducts", listProducts);
		return "index";
    }
	

	//Users:
	@GetMapping("/register")
	public String showRegistrationForm(Model model) {
		model.addAttribute("user", new User());
		return "user_form";
	}

	@GetMapping("/users")
	public String listUsers(Model model) {
		List<User> listUsers = userRepo.findAll();
		model.addAttribute("listUsers", listUsers);
		return "users";
	}
	
	@PostMapping("/process_register")
	public String processRegister(User user) {
		BCryptPasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
		String encodedPassword = passwordEncoder.encode(user.getPassword());
		user.setPassword(encodedPassword);
		user.setRole(ROLE);
		userRepo.save(user);
		return "user_success";
	}
	////////////////


	//Login:
	@GetMapping("/login")
	public String viewLoginPage() {
		return "login";
	}
	////////////////


	//Orders:
	@GetMapping("/makeOrder")
	public String makeOrder(@RequestParam("productid") long productID,
							@RequestParam("clientname") String clientname,
							Model model) {
		User user = userRepo.findByUsername(clientname);
		Order order = new Order();
		order.setProductID(productID);
		order.setClientID(user.getId());
		model.addAttribute("order", order);
		return "order_form";
	}

	@GetMapping("/orders")
	public String listOrders(Model model) {
		List<User> listUsers = userRepo.findAll();
		List<Order> listOrders = orderRepo.findAll();
		model.addAttribute("listUsers", listUsers);
		model.addAttribute("listOrders", listOrders);
		return "orders";
	}

	@GetMapping("/orders/{clientname}")
	public String listOrdersName(@PathVariable("clientname") String clientname, Model model) {
		User user = userRepo.findByUsername(clientname);
		List<Order> listOrders = orderRepo.findByClientID(user.getId());
		model.addAttribute("listOrders", listOrders);
		return "orders";
	}

	@PostMapping("/process_order")
	public String processOrder(Order order) {
		orderRepo.save(order);
		return "order_success";
	}
	////////////////


	//Products:
	@GetMapping("/makeProduct")
	public String makeProduct(Model model) {
		model.addAttribute("product", new Product());
		return "product_form";
	}

	@GetMapping("/products")
	public String listProducts(Model model) {
		List<User> listUsers = userRepo.findAll();
		List<Product> listProducts = productRepo.findAll();
		
		model.addAttribute("listUsers", listUsers);
		model.addAttribute("listProducts", listProducts);
		
		return "products";
	}

	@GetMapping("/product/{id}")
	public String productPage(HttpServletRequest request, @PathVariable("id") long id, Model model) {
		Product product = productRepo.findByID(id);
		model.addAttribute("product", product);
		return "product";
    }

	@GetMapping("/product/name")
	public String productPage(HttpServletRequest request,
							  @RequestParam(name="name") String name,
							  Model model) {
		List<Product> listProducts =  productRepo.findByNameContaining(name);
		model.addAttribute("listProducts", listProducts);
		return "index";
    }


	@PostMapping("/process_product")
	public String processProduct(Product product) {
		productRepo.save(product);
		return "product_success";
	}

	@GetMapping("/edit_product/{id}")
	public String showUpdateForm(@PathVariable("id") long id, Model model) {
		Product product = productRepo.findByID(id);
		
		model.addAttribute("product", product);
		return "product_edit";
	}

	@PostMapping("/update_product/{id}")
	public String updateProduct(@PathVariable("id") long id, Product product) {
		productRepo.save(product);
		return "product_success";
	}
	
	@GetMapping("/delete_product/{id}")
	public String deleteProduct(@PathVariable("id") long id, Model model) {
		Product product = productRepo.findByID(id);
		productRepo.delete(product);
		return "product_deleted";
	}
	////////////////

	@GetMapping("/noPermission")
	public String noPermission(){
		return "noPermission";
	}

	@GetMapping("/index")
	public String defaultAfterLogin(HttpServletRequest request, Model model) {
		List<Product> listProducts = productRepo.findAll();
		model.addAttribute("listProducts", listProducts);
        return "index";
    }

	@GetMapping("/advancedSearch")
	public String advancedSearch(HttpServletRequest request, Model model) {
		List<Product> listProducts = productRepo.findAll();
		model.addAttribute("listProducts", listProducts);
        return "advanced_search";
    }
	
}
